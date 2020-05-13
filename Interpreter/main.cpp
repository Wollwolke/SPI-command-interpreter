#include "Interpreter.h"
#include <iostream>
#include <string>

int main()
{
	auto interpreter = Interpreter("Registers.json");
	interpreter.interpretFile("A02_button_up.txt");
	/*std::string test = "\033[1;31mhi\033[0m";
	std::cout << test << std::endl;*/
}
