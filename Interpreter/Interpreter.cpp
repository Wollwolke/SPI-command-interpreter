#include "Interpreter.h"
#include <fstream>
#include <iostream>
#include <list>

Interpreter::Interpreter(std::string confName)
{
	std::ifstream file(confName);
	nlohmann::json jfile;
	file >> jfile;
	jconfig = jfile;
	file.close();
	registers = new Registers(jconfig);
	commands = new Commands(registers, jconfig);
}

void Interpreter::interpretFile(std::string fname)
{
	std::ifstream file(fname);
	std::string line;
	while (std::getline(file, line))
	{
		std::vector<std::string> bytes;
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
					std::string sub = line.substr(0, pos);
					if (sub.length() == 3)
					{
						sub = sub + "0";
					}
					bytes.push_back(sub);
					line.erase(0, pos + 1);
				}
				if (line.length() == 3) {
					line = line + "0";
				}
				bytes.push_back(line);
				//std::cout << line << "\n";
				// TODO: remove \ when \\ 
				if (bytes.size() > 1) {
					for (size_t i = 1; i < bytes.size(); i++)
					{
						commands->executeCommand(bytes[0], bytes[i]);
						std::cout << commands->interpret(jconfig, bytes.front());
					}	
				}
				else
				{
					std::cout << commands->interpret(jconfig, bytes.front());
				}
			}
		}
		else {
			std::cout << std::endl;
		}
	}
}
