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

std::string Commands::Command::interpret(std::string hcmd, nlohmann::json& config)
{
	return std::string();
}

Commands::Command::Command(bool strobe) :isstrobe(strobe) {}

Commands::StrobeCommand::StrobeCommand(std::string ilogic) :interpretation(ilogic), Command(true) {}

std::string Commands::StrobeCommand::interpret(std::string hcmd, nlohmann::json& config)
{
	return interpretation;
}

Commands::RegCommand::RegCommand(bool isread, std::string name):isread(isread),registername(name), Command(false){}

std::string Commands::RegCommand::interpret(std::string hcmd, nlohmann::json& config)
{
	auto jsonptr = config.at("interpret");
	if (jsonptr.at(registername).at("isfunc")) {
		interpretFunction(jsonptr.at(registername).at("func"));
	}
	else {
		interpretBits(jsonptr.at(registername).at("bits"));
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

