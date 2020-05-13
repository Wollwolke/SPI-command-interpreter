#pragma once
#include "Registers.h"
#include "Commands.h"
#include <string>
class Interpreter
{
private:
	Registers* registers;
	Commands* commands;

public:
	Interpreter(std::string confName);
	void interpretFile(std::string fName);
};
