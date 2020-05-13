#include "Commands.h"

Commands::Commands(nlohmann::json& config)
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


Commands::Command::Command(bool strobe) :isstrobe(strobe) {}

Commands::StrobeCommand::StrobeCommand(std::string ilogic) :interpretation(ilogic), Command(true) {}

Commands::RegCommand::RegCommand(bool isread, std::string name):isread(isread),registername(name), Command(false){}

