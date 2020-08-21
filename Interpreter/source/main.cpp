#include "Interpreter.h"
#include <iostream>
#include <string>
#include <Windows.h>

int main()
{
	try
	{
		/*HANDLE han = GetStdHandle(STD_OUTPUT_HANDLE);

		for (size_t i = 0; i < 100; i++)
		{
			SetConsoleTextAttribute(han, i);
			std::cout << i << " test\n";
		}*/
		auto interpreter = Interpreter("../data/Registers.json");
		interpreter.interpretFile("../data/presenter1/B01_power_on.txt");
		interpreter.interpretFile("../data/presenter1/A02_button_up.txt");
		interpreter.interpretFile("../data/presenter1/A03_button_down.txt");
		interpreter.interpretFile("../data/presenter1/B04_button_tab.txt");
		interpreter.interpretFile("../data/presenter1/B05_button_enter.txt");
	}
	catch (ERRORCODES)
	{}
	/*std::string test = "\033[1;31mhi\033[0m";
	std::cout << test << std::endl;*/
}
