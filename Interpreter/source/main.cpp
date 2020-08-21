#include "Interpreter.h"
#include <iostream>
#include <string>
#include <Windows.h>

int main(int argc, char* argv[])
{
	if (argc == 1) {
		std::cout << "no command line argmuments given";
	}
	else {
		auto interpreter = Interpreter("./data/Registers.json");
		try {
			for (size_t i = 1; i < argc; i++)
			{
				interpreter.interpretFile(argv[i]);
			}
		}
		catch (ERRORCODES){}
	}

	//try
	//{
	//	auto interpreter = Interpreter("../data/Registers.json");
	//	interpreter.interpretFile("../data/presenter1/B01_power_on.txt");
	//	interpreter.interpretFile("../data/presenter1/A02_button_up.txt");
	//	interpreter.interpretFile("../data/presenter1/A03_button_down.txt");
	//	interpreter.interpretFile("../data/presenter1/B04_button_tab.txt");
	//	interpreter.interpretFile("../data/presenter1/B05_button_enter.txt");
	//}
	//catch (ERRORCODES)
	//{}
}
