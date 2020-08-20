#include "Interpreter.h"
#include <iostream>
#include <string>

int main()
{
	try
	{
		auto interpreter = Interpreter("../data/Registers.json");
		interpreter.interpretFile("../data/presenter1/B01_power_on.txt");
		interpreter.interpretFile("../data/presenter1/A02_button_up.txt");
		// interpreter.interpretFile("A03_button_down.txt");
		// interpreter.interpretFile("B04_button_tab.txt");
		// interpreter.interpretFile("B05_button_enter.txt");
	}
	catch (ERRORCODES e)
	{
	}
	/*std::string test = "\033[1;31mhi\033[0m";
	std::cout << test << std::endl;*/
}
