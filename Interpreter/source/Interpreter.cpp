#include "Interpreter.h"
#include "exprtk.hpp"
#include <fstream>
#include <iostream>
#include <list>
#include <Windows.h>

Interpreter::Interpreter(std::string confName)
{
	std::ifstream file(confName);
	if (file.fail())
	{
		std::cerr << "Error while opening JSON file " << confName << std::endl;
		throw ERRORCODES::ERR_FATAL;
	}
	nlohmann::json jfile;
	try
	{
		file >> jfile;
		jconfig = jfile;
	}
	catch (...)
	{
		std::cerr << "ERROR while creating JSON file object" << std::endl;
		throw ERRORCODES::ERR_FATAL;
	}
	if (file.is_open())
	{
		file.close();
	}

	try
	{
		registers = new Registers(jconfig);
		commands = new Commands(registers, jconfig);
	}
	catch (ERRORCODES e)
	{
		if (e == ERRORCODES::ERR_JSONPARSER_REGISTER)
		{
			std::cerr << "Error while creating Registers from JSON file" << std::endl;
			throw ERRORCODES::ERR_FATAL;
		}
		else if (e == ERRORCODES::ERR_JSONPARSER_CMD)
		{
			std::cerr << "Error while creating Commands from JSON file" << std::endl;
			throw ERRORCODES::ERR_FATAL;
		}
		else
		{
			throw ERRORCODES::ERR_INTERPRET;
		}
	}
}

void Interpreter::interpretFile(std::string fname)
{
	// configure Console to accept ansi esc sequences
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleMode(hConsole, 7);

	std::ifstream file(fname);
	if (file.fail())
	{
		std::cerr << "Error while opening file " << fname << std::endl;
		throw ERRORCODES::ERR_INTERPRET;
	}
	std::string line;
	while (std::getline(file, line))
	{
		if (file.fail() || file.bad())
		{
			std::cerr << "Error unexpected line error" << std::endl;
			break;
		}
		std::vector<std::string> bytes;
		try
		{
			if (line != "")
			{
				if (line.at(0) == '#')
				{
					std::cout << "Comment found: " << line << std::endl;
				}
				else
				{
					//std::cout << "\n" << line << "\n";
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
					if (line.length() == 3)
					{
						line = line + "0";
					}
					bytes.push_back(line);
					//std::cout << line << "\n";
					// TODO: remove "\" when "\\"
					std::cout << "\n" << line << "\n";
					if (bytes.size() > 1)
					{
						for (size_t i = 1; i < bytes.size(); i++)
						{
							commands->executeCommand(bytes[0], bytes[i]);
							commands->interpret(jconfig, bytes.front());
						}
					}
					else
					{
						commands->interpret(jconfig, bytes.front());
					}
				}
			}
			else
			{
				std::cout << std::endl;
			}
		}
		catch (ERRORCODES)
		{
			//throw ERR_INTERPRET;
		}
		catch (...)
		{
			std::cerr << "Unexpected Error occured while Parsing input text file" << std::endl;
			throw ERRORCODES::ERR_INTERPRET;
		}
	}
}
