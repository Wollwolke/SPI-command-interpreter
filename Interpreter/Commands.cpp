#include "Commands.h"
#include "exprtk.hpp"
#include "utils.h"
#include <iostream>

Commands::Commands(Registers* reg, const nlohmann::json& config)
{
	try {
		auto it = config.at("commands");
		for (auto& command : it) {
			Command* nextcmd = nullptr;
			if (command.at("isstrobe")) {
				nextcmd = new StrobeCommand(command.at("logic"), reg);
			}
			else {
				nextcmd = new RegCommand(command.at("isread"), command.at("registername"), reg);
			}
			cmds.insert({ command.at("hex"), nextcmd });
		}
	}
	catch (nlohmann::json::exception e) {
		std::cerr << "Error while creating Command Objects from JSON" << std::endl;
		throw ERR_JSONPARSER_CMD;
	}
}

std::string Commands::interpret(nlohmann::json& jconfig, std::string hexcmd)
{
	if (cmds.count(hexcmd) == 0) {
		std::cerr << "Tried to interpret unknown command " << hexcmd << std::endl;
		throw ERR_INTERPRET;
	}
	Command* cmdPtr = cmds[hexcmd];
	try {
		return cmdPtr->interpret(jconfig);
	}
	catch (ERRORCODES e) {
		throw;
	}
}

void Commands::executeCommand(std::string cmd, std::string data)
{
	if (cmds.count(cmd) == 0) {
		std::cerr << "Tried to execute unknown command " << cmd << std::endl;
		throw ERR_EXECUTE_CMD;
	}
	Command* cmdPtr = cmds[cmd];
	if (!cmdPtr->isstrobe) {
		auto* regcmd = dynamic_cast<RegCommand *>(cmdPtr);
		if (!regcmd->isread) {
			try {
				regcmd->reg->writeByte(regcmd->registername, data);
			}
			catch (ERRORCODES e) {
				throw;
			}
		}
	}
}

std::string Commands::Command::interpret(nlohmann::json& config)
{
	return std::string();
}

Commands::Command::Command(bool strobe, Registers* reg) :isstrobe(strobe), reg(reg) {}

Commands::StrobeCommand::StrobeCommand(std::string ilogic, Registers* reg) :interpretation(ilogic), Command(true, reg) {}

std::string Commands::StrobeCommand::interpret(nlohmann::json& config)
{
	return "\x1B[37m" + interpretation + "\n";
}

Commands::RegCommand::RegCommand(bool isread, std::string name, Registers* reg):isread(isread),registername(name), Command(false, reg){}

std::string Commands::RegCommand::interpret(nlohmann::json& config)
{
	try {
		auto jsonptr = config.at("interpret").at(registername);

		std::string out = "";

		if (jsonptr.contains("ibits")) {
			out += interpretRegisters(jsonptr.at("ibits"));
		}
		else {
			if (jsonptr.contains("ibitsw") && !isread) {
				out += interpretRegisters(jsonptr.at("ibitsw"));
			}
			if (jsonptr.contains("ibitsr") && isread) {
				out += interpretRegisters(jsonptr.at("ibitsr"));
			}
		}
		return out;
	}
	catch (nlohmann::json::exception e) {
		std::cerr << "Error failed to load interpretation bits from JSON file-missing registername " << registername << std::endl;
		throw ERR_INTERPRET;
	}
	catch (ERRORCODES e) {
		throw;
	}
}

std::string Commands::RegCommand::interpretRegisters(nlohmann::json & ibits)
{
	std::string result = "";
	try {
		for (auto& bit : ibits) {
			if (bit.contains("highlight")) {
				result += "\x1B[31m";
			}
			else {
				result += "\x1B[37m";
			}
			if (bit.at("isfunc")) {
				result += interpretFunction(bit) + "\n";
			}
			else {
				result += interpretBits(bit) + "\n";
			}
		}
	}
	catch (nlohmann::json::exception e) {
		std::cerr << "Error Parsing isfunc flag in JSON File" << std::endl;
		throw ERR_INTERPRET;
	}
	catch (ERRORCODES e) {
		throw;
	}
	return result;
}

std::string Commands::RegCommand::interpretBits(nlohmann::json & ibit)
{
	std::string currentbitseq = "";
	try {
		std::vector<std::string> ibits = ibit.at("iwith");
		auto pos = 0;
		for (auto bit : ibits) {
			pos = 0;
			if ((pos = bit.find(":")) != std::string::npos) {
				try {
					currentbitseq += std::to_string(reg->readBit(bit.substr(0, pos), bit.substr(pos + 1, std::string::npos)));
				}
				catch (std::out_of_range) {
					std::cerr << "Error Namespace expression in JSON file interpret bits is invalid" << std::endl;
				}
			}
			else {
				currentbitseq += std::to_string(reg->readBit(registername, bit));
			}
		}
		if (ibit.at("ipret").contains("NA")) {
			//TODO: exceptions...
			int i = std::stoi(currentbitseq, nullptr, 2);
			std::string na = ibit.at("ipret").at("NA");
			na += " " + currentbitseq + " (" + std::to_string(i) + ")";
			return na;
		}
		return ibit.at("ipret").at(currentbitseq);
	}
	catch (nlohmann::json::exception) {
		std::cerr << "Error parsing bit interpretation in JSON file" << std::endl;
		throw ERR_INTERPRET;
	}
	catch (ERRORCODES e) {
		throw;
	}
}

std::string Commands::RegCommand::interpretFunction(nlohmann::json & ifunc)
{
	exprtk::symbol_table<double> symbols;
	std::string formula = ifunc.at("func");

	int pos = 0;
	std::vector<double> registerVal;
	for (auto iterator : ifunc.at("iwith").items()) {
		std::vector<std::string> ibits = ifunc.at("iwith").at(iterator.key());
		registerVal.push_back(intFromRegisters(ibits));
		symbols.add_constant(iterator.key(), registerVal[pos]);
		pos++;
	}

	exprtk::expression<double> expression;
	expression.register_symbol_table(symbols);

	exprtk::parser<double> parser;
	parser.compile(formula, expression);
	std::string ipret = ifunc.at("ipret");
	return  ipret + " " + std::to_string(expression.value());
}

double Commands::RegCommand::intFromRegisters(std::vector<std::string> ibits) {
	std::string currentbitseq = "";
	for (auto bit : ibits) {
		int pos = 0;
		if ((pos = bit.find(":")) != std::string::npos) {
			try {
				currentbitseq += std::to_string(reg->readBit(bit.substr(0, pos), bit.substr(pos + 1, std::string::npos)));
			}
			catch (std::out_of_range) {
				std::cerr << "Error Namespace expression in JSON file interpret bits is invalid" << std::endl;
			}
		}
		else {
			currentbitseq += std::to_string(reg->readBit(registername, bit));
		}
	}
	double res = std::stol(currentbitseq, nullptr, 2);
	return res;
}

