#include "Registers.h"
#include <iostream>
#include <fstream>

int Registers::readBit(std::string registername, std::string bitname)
{
	return regMap[registername]->readBit(bitname);
}

void Registers::writeByte(std::string registername, std::string hex)
{
	uint8_t byte = (uint8_t)strtol(hex.c_str(), NULL, 16);
	uint8_t bits[Registersize];
	for (size_t i = 1; i <= Registersize; ++i) {
		auto mask = (1 << i - 1);
		bits[Registersize - i] = (byte &mask) != 0;
		//00000001
	}
	auto *test = regMap[registername];
	test->writeRegister(bits);
}

Registers::Registers(const nlohmann::json &jfile)
{
	auto jregisters = jfile.at("registers");
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
		int erg = Values[NameIndices[name]];
		return erg;
	}
	std::cout << name << std::endl;
	return -1;
}

Registers::~Registers()
{
	for (const auto &[key, value] : regMap)
	{
		delete value;
	}
}
