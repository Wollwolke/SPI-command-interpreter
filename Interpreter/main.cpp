#include "Registers.h"
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <list>

int main() {
	auto registers = Registers();
	registers.createRegisters("Registers.json");
	std::ifstream file("A02_button_up.txt");

	std::string line;
	while (std::getline(file, line))
	{
		std::list<std::string> cmds;
		if (line != "") {
			if (line.at(0) == '#')
			{

			}
			else {
				size_t pos = 0;
				while ((pos = line.find(" ")) != std::string::npos) {
					cmds.push_back(line.substr(0, pos));
					line.erase(0, pos + 1);
				}
				cmds.push_back(line);
			}
		}
		for (auto& element : cmds) {
			if (element.length() == 3) {
				element = element + "0";
			}
		}
		std::cout << line << "\n";
		/*std::list<string> cmds;
		while ((pos = line.find(" ")) != std::string::npos) {

		}
		if(list==empty)*/
	}

}