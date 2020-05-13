#include "Registers.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>

Registers::Registers() {}

Registers::Register::Register(std::vector<std::string> wNames, std::vector<std::string> rNames, std::vector<int> dValues)
{
	for (size_t i = 0; i < Registersize; i++)
	{
		Values[i] = dValues[i];
		NameIndices.insert({wNames[i], i});
		NameIndices.insert({rNames[i], i});
	}
};

void Registers::Register::writeRegister(uint8_t bits[Registersize])
{
	for (size_t i = 0; i < Registersize; i++)
	{
		Values[i] = bits[i];
	}
}

int Registers::Register::readBit(std::string name)
{
	if (NameIndices.count(name) != 0)
	{
		return Values[NameIndices[name]];
	}
	return -1;
}

void Registers::createRegisters(nlohmann::json &jregisters)
{
	for (auto &element : jregisters)
	{
		//std::cout << i.value << "\n";
		try
		{
			std::string name = element.at("name");
			std::vector<int> values = element.at("values");
			std::vector<std::string> write = element.at("w");
			std::vector<std::string> read = element.at("r");
			Register *newReg = new Register(write, read, values);

			regMap.insert({name, newReg});
			//std::cout << name << ++cnt << "\n";
		}
		catch (nlohmann::detail::out_of_range exe)
		{
			std::cout << "json wrong format";
			break;
		}
	}
}

Registers::~Registers()
{
	for (const auto &[key, value] : regMap)
	{
		delete value;
	}
}
