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
	void interpret(nlohmann::json& jconfig, std::string hexcmd);
	void executeCommand(std::string cmd, std::string data = "");
protected:
	class Command {
	public:
		Registers* reg;
		bool isstrobe;
		virtual void interpret(nlohmann::json& config);
		Command(bool strobe, Registers* reg);
	};
	class StrobeCommand : public Command {
	public:
		std::string interpretation;
		StrobeCommand(std::string ilogic, Registers* reg);
		void interpret(nlohmann::json& config);
	};
	class RegCommand : public Command {
	public:
		bool isread;
		std::string registername;
		RegCommand(bool isread, std::string name, Registers* reg);
		void interpret(nlohmann::json& config);
	private:
		void interpretRegisters(nlohmann::json& ibits);
		std::string interpretBits(nlohmann::json& ibit);
		void generateBitString(std::vector<std::string>& ibits, std::string& currentbitseq);
		std::string interpretFunction(nlohmann::json& ifunc);
		double intFromRegisters(std::vector<std::string> a);
		//std::string createBitS(std::string[] iwith);
	};
	std::map<std::string, Command*> cmds;
};

