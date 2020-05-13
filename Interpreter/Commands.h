#pragma once
#include <map>
#include <nlohmann/json.hpp>
#include <string>

class Commands
{
public:
	Commands(nlohmann::json& config);
protected:
	class Command {
		bool isstrobe;
	public:
		Command(bool strobe);
	};
	class StrobeCommand : public Command {
	public:
		std::string interpretation;
		StrobeCommand(std::string ilogic);
	};
	class RegCommand : public Command {
	public:
		bool isread;
		std::string registername;
		RegCommand(bool isread, std::string name);
	};
	std::map<std::string, Command*> cmds;
};

