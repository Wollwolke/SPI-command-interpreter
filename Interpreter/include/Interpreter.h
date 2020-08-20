#pragma once
#include "Registers.h"
#include "utils.h"
#include <nlohmann/json.hpp>
#include "Commands.h"
#include <string>

class Interpreter
{
private:
	nlohmann::json jconfig;
	Registers *registers;
	Commands *commands;

public:
	Interpreter(std::string confName);
	void interpretFile(std::string fName);
};
