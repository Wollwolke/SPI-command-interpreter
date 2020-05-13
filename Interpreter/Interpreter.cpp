#include "Interpreter.h"
#include <nlohmann/json.hpp>
#include <fstream>

Interpreter::Interpreter(std::string confName) : registers(Registers())
{
    registers = Registers();
    commands = Commands();

    std::ifstream file(confName);
    nlohmann::json jfile;
    file >> jfile;

    auto jregisters = jfile.at("registers");
    registers.createRegisters(jregisters);
}
