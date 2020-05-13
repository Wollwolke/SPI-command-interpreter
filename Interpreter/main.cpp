#include "Interpreter.h"

int main()
{
	auto interpreter = Interpreter("Registers.json");
	interpreter.interpretFile("A02_button_up.txt");
}
