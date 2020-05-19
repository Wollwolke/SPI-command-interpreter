#include "Interpreter.h"
#include <iostream>
#include <string>

int main()
{
	auto interpreter = Interpreter("Registers.json");
	interpreter.interpretFile("B01_power_on.txt");
	/*std::string test = "\033[1;31mhi\033[0m";
	std::cout << test << std::endl;*/
}
