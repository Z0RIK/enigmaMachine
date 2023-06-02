#pragma once

#ifndef MACHINE_H
#define MACHINE_H

#include "enigma_rotor.h"

class Enigma_machine {
public:

	static Enigma_machine& getInstance()
	{
		static Enigma_machine instance;
		return instance;
	}

	Enigma_machine(Enigma_machine const&) = delete;
	void operator=(Enigma_machine const&) = delete;

	void setOffsets();
	void setOffsets(const std::vector<int>&);
	void setPlugboard();
	void setPlugboard(const Plugboard&);
	void addPair(const std::string&);
	void removePair(const std::string&);
	void setRotors();
	void setRotors(const std::vector<std::pair<std::string, std::string>>&);
	void setRotors(const std::vector<Rotor>&);
	void setReflector();
	void setReflector(const std::string&);
	void setReflector(const Rotor&);
	void setRotorCount(const uint8_t&);
	uint8_t getRotorCount();
	std::string encode(const std::string&);

	void print();

private:
	uint8_t mRotorCount;
	Rotor mReflector;
	Plugboard mPlugBoard;
	std::vector<Rotor> mRotors;
	std::vector<uint8_t> mOffsets;

	int8_t addOffset(const int8_t&, const uint8_t&);
	int8_t subOffset(const int8_t&, const uint8_t&);

	Enigma_machine();

	/*
	Enigma_machine(
		const uint8_t&,
		const std::string&,
		const std::vector < std::pair<std::string, std::string>>&);
	Enigma_machine(
		const uint8_t&,
		const Rotor&,
		const std::vector<Rotor>&);
	*/
};

#endif // !MACHINE_H