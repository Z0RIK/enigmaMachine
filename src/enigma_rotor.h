#pragma once

#ifndef ROTOR_H
#define ROTOR_H

#include <string>
#include <vector>
#include <array>
#include <unordered_set>
#include <iostream>

struct Plugboard {
	std::unordered_set<char> mAvailableChars;
	std::unordered_set<std::string> mPairs;
	std::array<uint8_t, 26> mHash;

	Plugboard();
	void addPair(std::string);
	void removePair(std::string);
	void reset();
	uint8_t operator[] (const int&);

private:
	void swap(const size_t&, const size_t&);
};

struct Rotor{

	std::array<uint8_t, 26> mHash;
	std::array<uint8_t, 2> mNotch;

	Rotor();
	Rotor(std::string, std::string);
	Rotor(const std::array<uint8_t, 26>&, const std::array<uint8_t, 2>& );
	Rotor(const Rotor& right);
	Rotor& operator= (const Rotor&);
	uint8_t operator[] (const int&);
	uint8_t at(const int&);
	bool turn(const int&);
};

#endif // !ROTOR_H