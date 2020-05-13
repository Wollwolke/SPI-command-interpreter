#pragma once
#include <map>

class Commands
{
public:
	Commands();
private:
	class Command {
	public:
		bool isstrobe;
	};
	class Strobecommand : public Command {
		std::string interpretation;
	};
	class RegCommand : public Command {
		bool isread;
		std::string registername;
	};
	std::map<std::string, Command> cmds;
};

