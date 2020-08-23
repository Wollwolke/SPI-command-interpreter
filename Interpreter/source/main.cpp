#include "Interpreter.h"
#include <iostream>
#include <string>
#include <filesystem>
#include <Windows.h>

int main(int argc, char* argv[])
{
	/*
	if (argc == 1) {
		std::cout << "+++++no command line argmuments given the txt logfiles in the data folder will be recursively analysed this can lead to wrong outputs due to wrong execution order of for example startup files+++++\n";
		auto interpreter = Interpreter("./data/Registers.json");
		for (auto& iterator : std::filesystem::recursive_directory_iterator("data")) {
			if (iterator.path().extension() == ".txt") {
				interpreter.interpretFile(iterator.path().string());
			}
		}
	}
	else {
		auto interpreter = Interpreter("./data/Registers.json");
		try {
			for (int i = 1; i < argc; i++)
			{
				interpreter.interpretFile(argv[i]);
			}
		}
		catch (ERRORCODES){
		}
	}

	std::cout << "\npress any key to continue";
	std::cin.get();*/

	try
	{
		auto interpreter = Interpreter("../data/Registers.json");
		interpreter.interpretFile("../data/presenter2/B05_key_audioLowerVolume.txt");
	//	interpreter.interpretFile("../data/presenter1/A02_button_up.txt");
	//	interpreter.interpretFile("../data/presenter1/A03_button_down.txt");
	//	interpreter.interpretFile("../data/presenter1/B04_button_tab.txt");
	//	interpreter.interpretFile("../data/presenter1/B05_button_enter.txt");
	}
	catch (ERRORCODES)
	{}
}
