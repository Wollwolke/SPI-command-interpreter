#include "Interpreter.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <list>

Interpreter::Interpreter(std::string confName)
{
	std::ifstream file(confName);
	nlohmann::json jfile;
	file >> jfile;

	registers = new Registers(jfile);
	commands = new Commands(jfile);
}

void Interpreter::interpretFile(std::string fname)
{
	std::ifstream file(fname);
	std::list<std::string> bytes;
	std::string line;
	while (std::getline(file, line))
	{
		if (line != "")
		{
			if (line.at(0) == '#')
			{
				std::cout << "Comment found" << std::endl;
			}
			else
			{
				size_t pos = 0;
				while ((pos = line.find(" ")) != std::string::npos)
				{
					bytes.push_back(line.substr(0, pos));
					line.erase(0, pos + 1);
				}
				bytes.push_back(line);
			}
		}
		for (auto &element : bytes)
		{
			if (element.length() == 3)
			{
				element = element + "0";
			}
		}
		std::cout << line << "\n";
		// do smth
	}
