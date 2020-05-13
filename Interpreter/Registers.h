#pragma once
#include <string>
#include <map>
#include <vector>

class Registers
{
private:
	static const int Registersize = 8;
	class Register
	{
	public:
		std::map<std::string, uint8_t> NameIndices;
		int Values[Registersize];

		Register(std::vector<std::string> wNames, std::vector<std::string> rNames, std::vector<int> dValues);

		void writeRegister(uint8_t bits[Registersize]);

		int readBit(std::string name);
	};
	std::map<std::string, Register *> regMap;

public:
	Registers();
	void createRegisters(nlohmann::json &jregisters);
	~Registers();
};
