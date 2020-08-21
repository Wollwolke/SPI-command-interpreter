#include "Commands.h"
#include "exprtk.hpp"
#include "utils.h"
#include <iostream>
#include <Windows.h>

Commands::Commands(Registers *reg, const nlohmann::json &config)
{
	try
	{
		auto it = config.at("commands");
		for (auto &command : it)
		{
			Command *nextcmd = nullptr;
			if (command.at("isstrobe"))
			{
				nextcmd = new StrobeCommand(command.at("logic"), reg);
			}
			else
			{
				nextcmd = new RegCommand(command.at("isread"), command.at("registername"), reg);
			}
			cmds.insert({command.at("hex"), nextcmd});
		}
	}
	catch (nlohmann::json::exception e)
	{
		std::cerr << "Error while creating Command Objects from JSON" << std::endl;
		throw ERRORCODES::ERR_JSONPARSER_CMD;
	}
}

void Commands::interpret(nlohmann::json &jconfig, std::string hexcmd)
{
	if (cmds.count(hexcmd) == 0)
	{
		std::cerr << "Tried to interpret unknown command " << hexcmd << std::endl;
		throw ERRORCODES::ERR_INTERPRET;
	}
	Command *cmdPtr = cmds[hexcmd];
	try
	{
		cmdPtr->interpret(jconfig);
	}
	catch (ERRORCODES e)
	{
		throw e;
	}
}

void Commands::executeCommand(std::string cmd, std::string data)
{
	if (cmds.count(cmd) == 0)
	{
		std::cerr << "Tried to execute unknown command " << cmd << std::endl;
		throw ERRORCODES::ERR_EXECUTE_CMD;
	}
	Command *cmdPtr = cmds[cmd];
	if (!cmdPtr->isstrobe)
	{
		auto *regcmd = dynamic_cast<RegCommand *>(cmdPtr);
		if (!regcmd->isread)
		{
			try
			{
				regcmd->reg->writeByte(regcmd->registername, data);
			}
			catch (ERRORCODES e)
			{
				throw e;
			}
		}
	}
}

void Commands::Command::interpret(nlohmann::json &config)
{
	return;
}

Commands::Command::Command(bool strobe, Registers *reg) : isstrobe(strobe), reg(reg) {}

Commands::StrobeCommand::StrobeCommand(std::string ilogic, Registers *reg) : interpretation(ilogic), Command(true, reg) {}

void Commands::StrobeCommand::interpret(nlohmann::json &config)
{
	std::cout << interpretation << "\n";
}

Commands::RegCommand::RegCommand(bool isread, std::string name, Registers *reg) : isread(isread), registername(name), Command(false, reg) {}

void Commands::RegCommand::interpret(nlohmann::json &config)
{
	try
	{
		auto jsonptr = config.at("interpret").at(registername);

		if (jsonptr.contains("ibits"))
		{
			interpretRegisters(jsonptr.at("ibits"));
		}
		else
		{
			if (jsonptr.contains("ibitsw") && !isread)
			{
				interpretRegisters(jsonptr.at("ibitsw"));
			}
			if (jsonptr.contains("ibitsr") && isread)
			{
				interpretRegisters(jsonptr.at("ibitsr"));
			}
		}
	}
	catch (nlohmann::json::exception e)
	{
		std::cerr << "Error failed to load interpretation bits from JSON file - missing registername " << registername << std::endl;
		throw ERRORCODES::ERR_INTERPRET;
	}
	catch (ERRORCODES e)
	{
		throw e;
	}
}

void Commands::RegCommand::interpretRegisters(nlohmann::json &ibits)
{
	/*HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleMode(hConsole, 7);*/
	try
	{
		for (auto &bit : ibits)
		{
			std::string result = "";
			if (bit.contains("highlight"))
			{
				/*std::string colorval = bit.at("highlight");
				int color = std::stoi(colorval);
				SetConsoleTextAttribute(hConsole, (color%13)+1);*/
				result += "\x1B[";
				result += bit.at("highlight");
				result += "m\x1B[1m";
			}
			if (bit.at("isfunc"))
			{
				result += interpretFunction(bit) + "\x1B[37m\n";
				//SetConsoleTextAttribute(hConsole, 15);
			}
			else
			{
				result += interpretBits(bit) + "\x1B[37m\n";
				//SetConsoleTextAttribute(hConsole, 15);
			}
			std::cout << result;
		}
	}
	catch (nlohmann::json::exception e)
	{
		std::cerr << "Error Parsing isfunc flag in JSON File " << registername << std::endl;
		throw ERRORCODES::ERR_INTERPRET;
	}
	catch (ERRORCODES e)
	{
		throw e;
	}
}

std::string Commands::RegCommand::interpretBits(nlohmann::json &ibit)
{
	std::string currentbitseq = "";
	try
	{
		std::vector<std::string> ibits = ibit.at("iwith");
		generateBitString(ibits, currentbitseq);
		if (ibit.at("ipret").contains("NA"))
		{
			//TODO: exceptions...
			int i = std::stoi(currentbitseq, nullptr, 2);
			std::string na = ibit.at("ipret").at("NA");
			na += " " + currentbitseq + " (" + std::to_string(i) + ")";
			return na;
		}
		return ibit.at("ipret").at(currentbitseq);
	}
	catch (nlohmann::json::exception)
	{
		std::cerr << "Error parsing bit interpretation in JSON file at " << registername << std::endl;
		throw ERRORCODES::ERR_INTERPRET;
	}
	catch (ERRORCODES e)
	{
		throw e;
	}
}

void Commands::RegCommand::generateBitString(std::vector<std::string>& ibits, std::string& currentbitseq)
{
	auto pos = 0;
	for (auto bit : ibits)
	{
		pos = 0;
		if ((pos = bit.find(":")) != std::string::npos)
		{
			try
			{
				currentbitseq += std::to_string(reg->readBit(bit.substr(0, pos), bit.substr(pos + 1, std::string::npos)));
			}
			catch (std::out_of_range)
			{
				std::cerr << "Error Namespace expression in JSON file interpret bits is invalid" << std::endl;
			}
		}
		else
		{
			currentbitseq += std::to_string(reg->readBit(registername, bit));
		}
	}
}

std::string Commands::RegCommand::interpretFunction(nlohmann::json &ifunc)
{
	exprtk::symbol_table<double> symbols;
	std::string formula = ifunc.at("func");

	int pos = 0;
	std::vector<double> registerVal;
	for (auto iterator : ifunc.at("iwith").items())
	{
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
	return ipret + " " + std::to_string(expression.value());
}

double Commands::RegCommand::intFromRegisters(std::vector<std::string> ibits)
{
	std::string currentbitseq = "";
	generateBitString(ibits, currentbitseq);
	double res = std::stol(currentbitseq, nullptr, 2);
	return res;
}
