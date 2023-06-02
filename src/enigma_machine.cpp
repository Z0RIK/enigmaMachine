#pragma once
#include "enigma_machine.h"

static Enigma_machine* enigma;

void Enigma_machine::print()
{
	// for debugging

	std::cout << "mRotorCount = " << (int)mRotorCount << std::endl;
	for (size_t i = 0; i < mRotorCount; i++)
	{
		std::cout << "Rotor " << i << ": ";
		for (auto& c : mRotors[i].mHash)
			std::cout << char(c + 'A');
		std::cout << '\t' << (char)(mRotors[i].mNotch[0] + 'A') << (char)(mRotors[i].mNotch[1] + 'A');
		std::cout << "\t" << (int)mOffsets[i] << std::endl;
	}

	std::cout << "mReflector: ";
	for (auto& it : mReflector.mHash)
		std::cout << char(it + 'A');
	std::cout << std::endl;

	std::cout << "mPlugboard: ";
	for (auto& it : mPlugBoard.mHash)
		std::cout << char(it + 'A');
	std::cout << std::endl;
}

Enigma_machine::Enigma_machine() 
{
	mRotorCount = 3;
	mOffsets.resize(mRotorCount, 0);
	mRotors.resize(mRotorCount, Rotor());
	setReflector();
	setOffsets();
	setRotors();
}

/*
Enigma_machine::Enigma_machine(
	const uint8_t& numberOfRotors,
	const std::string& reflectorString,
	const std::vector<std::pair<std::string, std::string>>& rotorStrings)
{
	mRotorCount = numberOfRotors;
	mOffsets.resize(mRotorCount, 0);
	mRotors.resize(mRotorCount, Rotor());
	setReflector(reflectorString);
	setRotors(rotorStrings);
}

Enigma_machine::Enigma_machine(
	const uint8_t& numberOfRotors,
	const Rotor& reflector,
	const std::vector<Rotor>& rotors)
{
	mRotorCount = numberOfRotors;
	mOffsets.resize(mRotorCount, 0);
	mRotors.resize(mRotorCount, Rotor());
	setReflector(reflector);
	setRotors(rotors);
}
*/


void Enigma_machine::setOffsets() 
{
	for (uint8_t i = 0; i < mRotorCount; i++)
		mOffsets[i] = 0;
}

void Enigma_machine::setOffsets(const std::vector<int>& offsetsVector)
{
	if (offsetsVector.size() != mRotorCount)
		return;

	for (size_t i = 0; i < mRotorCount; i++) {
		mOffsets[i] = offsetsVector[i] % 26;
	}
}

void Enigma_machine::setPlugboard()
{
	mPlugBoard.reset();
}

void Enigma_machine::setPlugboard(const Plugboard& plugboard)
{
	mPlugBoard = plugboard;
}

void Enigma_machine::addPair(const std::string& pair)
{
	mPlugBoard.addPair(pair);
}

void Enigma_machine::removePair(const std::string& pair)
{
	mPlugBoard.removePair(pair);
}

void Enigma_machine::setReflector()
{
	mReflector = Rotor("ABCDEFGHIJKLMNOPQRSTUVWXYZ", "AA");
}

void Enigma_machine::setReflector(const std::string& reflectorString)
{
	mReflector = Rotor(reflectorString, "AA");
}

void Enigma_machine::setReflector(const Rotor& rotor)
{
	mReflector = rotor;
}

void Enigma_machine::setRotors()
{
	Rotor temp("ABCDEFGHIJKLMNOPQRSTUVWXYZ", "AA");

	for (size_t i = 0; i < mRotorCount; i++)
		mRotors[i] = temp;
}

void Enigma_machine::setRotors(const std::vector<std::pair<std::string, std::string>>& rotorStrings)
{
	for (size_t i = 0; i < mRotorCount || i < rotorStrings.size(); i++)
		mRotors[i] = Rotor(rotorStrings[i].first, rotorStrings[i].second);
}

void Enigma_machine::setRotors(const std::vector<Rotor>& rotorsVector)
{
	for (size_t i = 0; i < mRotorCount || i < rotorsVector.size(); i++)
		mRotors[i] = rotorsVector[i];
}

void Enigma_machine::setRotorCount(const uint8_t& numberOfRotors)
{
	mRotorCount = numberOfRotors;
	mRotors.resize(mRotorCount);
	mOffsets.resize(mRotorCount);
}

int8_t Enigma_machine::addOffset(const int8_t& character, const uint8_t&  offset)
{
	return (character + offset) % 26;
}

int8_t Enigma_machine::subOffset(const int8_t& character, const uint8_t& offset)
{
	return (character + 26 - offset) % 26;
}

uint8_t Enigma_machine::getRotorCount()
{
	return mRotorCount;
}

std::string Enigma_machine::encode(const std::string& input) 
{
	std::string result;
	result.reserve(input.size());
	int8_t c{}, j{};
	bool turnOver;
	for (size_t i = 0; i < input.size(); i++)
	{
		if (!std::isalpha((uint8_t)input[i])) continue;
		c = std::toupper(input[i]) - 'A';
		c = mPlugBoard[c];

		turnOver = true; // FIRST ROTOR ALWAYS ROTATES

		for (j = mRotorCount - 1; j >= 0; j--) 
		{ 
			// ROTATION OF ROTORS
			if (turnOver)
			{
				turnOver = mRotors[j].turn(mOffsets[j]);
				mOffsets[j]++;
				mOffsets[j] = mOffsets[j] % 26;
			}
			// FROM RIGHT TO LEFT
			c = addOffset(c, mOffsets[j]);
			c = mRotors[j][c];
			c = subOffset(c, mOffsets[j]);
		}

		c = mReflector[c];

		for (j = 0; j < mRotorCount; j++) 
		{
			// FROM LEFT TO RIGHT
			c = addOffset(c, mOffsets[j]);
			c = mRotors[j].at(c);
			c = subOffset(c, mOffsets[j]);
		}

		c = mPlugBoard[c];
		result.push_back(c + 'A');
	}

	return result;
}