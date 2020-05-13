#pragma once
#include <map>
#include <nlohmann/json.hpp>
#include <string>

class Commands
{
public:
	Commands(const nlohmann::json& config);
protected:
	class Command {
		bool isstrobe;
		virtual std::string interpret(std::string hcmd, nlohmann::json& config);
	public:
		Command(bool strobe);
	};
	class StrobeCommand : public Command {
	public:
		std::string interpretation;
		StrobeCommand(std::string ilogic);
		std::string interpret(std::string hcmd, nlohmann::json& config);
	};
	class RegCommand : public Command {
	public:
		bool isread;
		std::string registername;
		RegCommand(bool isread, std::string name);
		std::string interpret(std::string hcmd, nlohmann::json& config);
	private:
		std::string interpretBits(nlohmann::json& ibits);
		std::string interpretFunction(nlohmann::json& ifunc);
	};
	std::map<std::string, Command*> cmds;
};

