#pragma once
#include "pch.h"
using std::vector;
using std::distance;

bool isInputKeyword(std::string input, std::vector<std::string>* keyw)
{
	std::vector<std::string>::iterator i;
	for (i = keyw->begin(); i < keyw->end(); i++) {
		std::string cache = *i;
		if (cache == input) {
			return true;
		}
	}
	return false;
}

std::string decToHex(unsigned long int decimal_value,int bits)
{
	std::stringstream ss;
	ss << std::hex << decimal_value; // int decimal_value
	std::string res(ss.str());
	while ((bits / 4 ) > res.length()+1) {
		res.insert(0, 1, '0');
	}
	return res;
}

std::string decToHex(std::string decimal_value, int bits)
{
	unsigned long int _decimal_value = std::stoi(decimal_value);
	std::stringstream ss;
	ss << std::hex << _decimal_value; // int decimal_value
	std::string res(ss.str());
	while ((bits / 4) > res.length() + 1) {
		res.insert(0, 1, '0');
	}
	return res;
}