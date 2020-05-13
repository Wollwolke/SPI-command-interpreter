#include "Interpreter.h"
#include <nlohmann/json.hpp>
#include <fstream>

Interpreter::Interpreter(std::string confName)
{
    std::ifstream file(confName);
    nlohmann::json jfile;
    file >> jfile;

    auto jregisters = jfile.at("commands");

    registers = new Registers(jfile);
    commands = new Commands(jfile);
}

void Interpreter::interpretFile(std::string fName)
{
}
