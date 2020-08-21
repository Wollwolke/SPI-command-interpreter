#include "Registers.h"
#include "utils.h"
#include <iostream>
#include <fstream>

Registers::Registers(const nlohmann::json &jfile)
{
	try
	{
		auto jregisters = jfile.at("registers");
		for (auto &element : jregisters)
		{
			std::string name = element.at("name");
			std::vector<int> values = element.at("values");
			std::vector<std::string> write = element.at("w");
			std::vector<std::string> read = element.at("r");
			Register *newReg = new Register(write, read, values);

			regMap.insert({name, newReg});
			//std::cout << name << ++cnt << "\n";
		}
	}
	catch (nlohmann::json::exception e)
	{
		std::cerr << "Error at parsing json file to create Registers Object" << std::endl;
		throw ERRORCODES::ERR_JSONPARSER_REGISTER;
	}
	catch (ERRORCODES e)
	{
		throw ERRORCODES::ERR_JSONPARSER_REGISTER;
	}
}

int Registers::readBit(std::string registername, std::string bitname)
{
	if (regMap.count(registername) == 0)
	{
		std::cerr << "Tried to read nonexistent bit at " << registername << std::endl;
		throw ERRORCODES::ERR_INTERPRET;
	}
	try
	{
		return regMap[registername]->readBit(bitname);
	}
	catch (ERRORCODES e)
	{
		throw;
	}
}

void Registers::writeByte(std::string registername, std::string hex)
{
	uint8_t byte = (uint8_t)strtol(hex.c_str(), NULL, 16);
	uint8_t bits[Registersize];
	for (size_t i = 1; i <= Registersize; ++i)
	{
		auto mask = (1 << i - 1);
		bits[Registersize - i] = (byte & mask) != 0;
		//00000001
	}
	if (regMap.count(registername) == 0)
	{
		std::cerr << "Tried to write nonexistent byte at " << registername << std::endl;
		throw ERRORCODES::ERR_EXECUTE_CMD;
	}
	auto *registerP = regMap[registername];
	registerP->writeRegister(bits);
}

Registers::Register::Register(std::vector<std::string> wNames, std::vector<std::string> rNames, std::vector<int> dValues)
{
	try
	{
		for (size_t i = 0; i < Registersize; i++)
		{
			Values[i] = dValues[i];
			NameIndices.insert({wNames[i], i});
			NameIndices.insert({rNames[i], i});
		}
	}
	catch (std::out_of_range e)
	{
		std::cerr << "Error mismatched dimensions while creating Register with" << std::endl;
		throw ERRORCODES::ERR_REGISTER;
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
	std::cerr << "Error trying to read Bit at " << name << std::endl;
	throw ERRORCODES::ERR_INTERPRET;
}

Registers::~Registers()
{
	for (const auto &[key, value] : regMap)
	{
		delete value;
	}
}
