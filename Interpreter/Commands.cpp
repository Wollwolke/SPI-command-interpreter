#include "Commands.h"

Commands::Commands(const nlohmann::json& config)
{
	auto it = config.at("commands");
	for (auto& command : it) {
		Command* nextcmd;
		if (command.at("isstrobe")) {
			nextcmd = new StrobeCommand(command.at("logic"));
		}
		else {
			nextcmd = new RegCommand(command.at("isread"), command.at("registername"));
		}
		cmds.insert({ command.at("hex"), nextcmd });
	}
}

std::string Commands::interpret(nlohmann::json& jconfig, std::string hexcmd)
{
	Command* cmdPtr = cmds[hexcmd];
	return cmdPtr->interpret(jconfig);
}

std::string Commands::Command::interpret(nlohmann::json& config)
{
	return std::string();
}

Commands::Command::Command(bool strobe) :isstrobe(strobe) {}

Commands::StrobeCommand::StrobeCommand(std::string ilogic) :interpretation(ilogic), Command(true) {}

std::string Commands::StrobeCommand::interpret(nlohmann::json& config)
{
	return interpretation;
}

Commands::RegCommand::RegCommand(bool isread, std::string name):isread(isread),registername(name), Command(false){}

std::string Commands::RegCommand::interpret(nlohmann::json& config)
{
	// TODO: adjust to new json layout
	auto jsonptr = config.at("interpret");
	if (jsonptr.at(registername).at("isfunc")) {
		return interpretFunction(jsonptr.at(registername).at("func"));
	}
	else {
		return interpretBits(jsonptr.at(registername).at("bits"));
	}
}

std::string Commands::RegCommand::interpretBits(nlohmann::json & ibits)
{
	return std::string();
}

std::string Commands::RegCommand::interpretFunction(nlohmann::json & ifunc)
{
	return std::string();
}

