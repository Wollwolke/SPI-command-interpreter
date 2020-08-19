#include "Interpreter.h"
#include <iostream>
#include <string>

int main()
{
	try {
		auto interpreter = Interpreter("Registers.json");
		interpreter.interpretFile("B01_power_on.txt");
		interpreter.interpretFile("A02_button_up.txt");
	}
	catch (ERRORCODES e) {

	}
	/*std::string test = "\033[1;31mhi\033[0m";
	std::cout << test << std::endl;*/
}
