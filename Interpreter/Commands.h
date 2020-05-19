#pragma once
#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include "Registers.h"

class Commands
{
public:
	Commands(Registers* reg, const nlohmann::json& config);
	std::string interpret(nlohmann::json& jconfig, std::string hexcmd);
	void executeCommand(std::string cmd, std::string data = "");
protected:
	class Command {
	public:
		Registers* reg;
		bool isstrobe;
		virtual std::string interpret(nlohmann::json& config);
		Command(bool strobe, Registers* reg);
	};
	class StrobeCommand : public Command {
	public:
		std::string interpretation;
		StrobeCommand(std::string ilogic, Registers* reg);
		std::string interpret(nlohmann::json& config);
	};
	class RegCommand : public Command {
	public:
		bool isread;
		std::string registername;
		RegCommand(bool isread, std::string name, Registers* reg);
		std::string interpret(nlohmann::json& config);
	private:
		std::string interpretRegisters(nlohmann::json& ibits);
		std::string interpretBits(nlohmann::json& ibit);
		std::string interpretFunction(nlohmann::json& ifunc);
		//std::string createBitS(std::string[] iwith);
	};
	std::map<std::string, Command*> cmds;
};

