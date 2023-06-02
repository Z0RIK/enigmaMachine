#include "enigma_rotor.h"

Plugboard::Plugboard()
{
	reset();
}

void Plugboard::addPair(std::string pair)
{
	pair[0] = std::toupper(pair[0]);
	pair[1] = std::toupper(pair[1]);

	if (pair.size() != 2 || !std::isalpha(pair[0]) || !std::isalpha(pair[1]))
	{
		std::cerr << "WARNING::PLUGBOARD::Input string should consist of two characters" << std::endl;
		return;
	}
	if (pair[0] == pair[1])
	{
		std::cerr << "WARNING::PLUGBOARD::Same character cant be used more than once" << std::endl;
		return;
	}
	if (!mAvailableChars.count(pair[0]) || !mAvailableChars.count(pair[1]))
	{
		std::cerr << "WARNING::PLUGBOARD::Pair " << pair[0] << pair[1] << " has not been added because one or both characters are already in use" << std::endl;
		return;
	}


	mAvailableChars.erase(pair[0]);
	mAvailableChars.erase(pair[1]);

	mPairs.insert(pair);

	swap(pair[0] - 'A', pair[1] - 'A');
}

void Plugboard::removePair(std::string pair)
{
	pair[0] = std::toupper(pair[0]);
	pair[1] = std::toupper(pair[1]);

	if (!mPairs.count(pair))
	{
		std::cerr << "WARNING::PLUGBOARD::Trying to remove pair that does not exist" << std::endl;
		return;
	}

	mPairs.erase(pair);

	mAvailableChars.insert(pair[0]);
	mAvailableChars.insert(pair[1]);

	swap(pair[0] - 'A', pair[1] - 'A');
}

void Plugboard::reset()
{
	mAvailableChars.clear();
	mPairs.clear();

	for (size_t i = 0; i < 26; i++)
	{
		mHash[i] = (uint8_t)i;
		mAvailableChars.insert((char)('A' + i));
	}
}

uint8_t Plugboard::operator[] (const int& index)
{
	if (index < 0 || 25 < index)
	{
		std::cerr << "ERROR::PLUGBOARD::Index is out of range: " << index << std::endl;
		return 0;
	}

	return mHash[index];
}

void Plugboard::swap(const size_t& index1, const size_t& index2)
{
	uint8_t temp = mHash[index1];
	mHash[index1] = mHash[index2];
	mHash[index2] = temp;
}


Rotor::Rotor() {
	mHash = {};
	mNotch = {};
}

Rotor::Rotor(std::string h, std::string n) {
	h.resize(26, 'A');
	n.resize(2, 'A');
	for (auto& it : h)
	{
		it = std::toupper(it);
		if (it < 'A' || it > 'Z')
			std::cerr << "WARNING::ROTOR::Wrong hash string: " << h << std::endl;
	}

	for (auto& it : n)
	{
		it = std::toupper(it);
		if (it < 'A' || it > 'Z')
			std::cerr << "WARNING::ROTOR::Wrong notch values: " << n << std::endl;
	}

	mHash = {};
	mNotch = {};

	for (size_t i = 0; i < 26; i++)
	{
		mHash[i] = h[i] - 'A';
	}
	mNotch[0] = n[0] - 'A';
	mNotch[1] = n[1] - 'A';
}

Rotor::Rotor(const std::array<uint8_t, 26>& h, const std::array<uint8_t, 2>& n) {
	mHash = h;
	mNotch = n;
}

Rotor::Rotor(const Rotor& right) {
	mHash = right.mHash;
	mNotch = right.mNotch;
}

Rotor& Rotor::operator= (const Rotor& right) {
	if (this == &right)
		return *this;

	mHash = right.mHash;
	mNotch = right.mNotch;
	return *this;
}

uint8_t Rotor::operator[] (const int& index) {
	if (index < 0 || 25 < index) {
		std::cerr << "ERROR::ROTOR::Index is out of range: " << index << std::endl;
		return 0;
	}
	return mHash[index];
}

uint8_t Rotor::at(const int& index) {
	if (index < 0 || 25 < index) {
		std::cerr << "ERROR::ROTOR::Index is out of range: " << index << std::endl;
		return 0;
	}

	for (uint8_t i = 0; i < 26; i++) {
		if (mHash[i] == index)
			return i;
	}
}

bool Rotor::turn(const int& index) {
	if (index < 0 || 25 < index) {
		std::cerr << "ERROR::ROTOR::Indes is out of range: " << index << std::endl;
		return 0;
	}
	if (index == mNotch[0] || index == mNotch[1])
		return true;
	else
		return false;
}