#include "Commands.h"

Commands::Commands(Registers* reg, const nlohmann::json& config)
{
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

std::string Commands::interpret(nlohmann::json& jconfig, std::string hexcmd)
{
	Command* cmdPtr = cmds[hexcmd];
	return cmdPtr->interpret(jconfig);
}

void Commands::executeCommand(std::string cmd, std::string data)
{
	Command* cmdPtr = cmds[cmd];
	if (!cmdPtr->isstrobe) {
		auto* regcmd = dynamic_cast<RegCommand *>(cmdPtr);
		if (!regcmd->isread) {
			regcmd->reg->writeByte(regcmd->registername, data);
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
	return interpretation + "\n"; 
}

Commands::RegCommand::RegCommand(bool isread, std::string name, Registers* reg):isread(isread),registername(name), Command(false, reg){}

std::string Commands::RegCommand::interpret(nlohmann::json& config)
{
	// TODO: adjust to new json layout
	auto jsonptr = config.at("interpret").at(registername);
	if (jsonptr.contains("ibits")) {
		return interpretRegisters(jsonptr.at("ibits"));
	}
	else {
		if (jsonptr.contains("ibitsw") && !isread) {
			return interpretRegisters(jsonptr.at("ibitsw"));
		}
		if (jsonptr.contains("ibitsr") && isread) {
			return interpretRegisters(jsonptr.at("ibitsr"));
		}
	}
}

std::string Commands::RegCommand::interpretRegisters(nlohmann::json & ibits)
{
	std::string result = "";
	for (auto& bit : ibits) {
		if (bit.at("isfunc")) {
			result += interpretFunction(bit) + "\n";
		}
		else {
			result += interpretBits(bit) + "\n";
		}
	}
	return result;
}

std::string Commands::RegCommand::interpretBits(nlohmann::json & ibit)
{
	std::string currentbitseq = "";
	std::vector<std::string> ibits = ibit.at("iwith");
	auto pos = 0;
	for (auto bit : ibits) {
		pos = 0;
		if ((pos = bit.find(":")) != std::string::npos) {
			currentbitseq += std::to_string(reg->readBit(bit.substr(0, pos), bit.substr(pos, std::string::npos)));
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

std::string Commands::RegCommand::interpretFunction(nlohmann::json & ifunc)
{
	return "hההה function kann i net";
}

