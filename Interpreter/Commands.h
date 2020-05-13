#pragma once
#include <map>
#include <nlohmann/json.hpp>
#include <string>

class Commands
{
public:
	Commands(const nlohmann::json& config);
	std::string interpret(nlohmann::json& jconfig, std::string hexcmd);
protected:
	class Command {
		bool isstrobe;
	public:
		virtual std::string interpret(nlohmann::json& config);
		Command(bool strobe);
	};
	class StrobeCommand : public Command {
	public:
		std::string interpretation;
		StrobeCommand(std::string ilogic);
		std::string interpret(nlohmann::json& config);
	};
	class RegCommand : public Command {
	public:
		bool isread;
		std::string registername;
		RegCommand(bool isread, std::string name);
		std::string interpret(nlohmann::json& config);
	private:
		std::string interpretBits(nlohmann::json& ibits);
		std::string interpretFunction(nlohmann::json& ifunc);
	};
	std::map<std::string, Command*> cmds;
};

