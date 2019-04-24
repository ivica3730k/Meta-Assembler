#pragma once
#include "pch.h"
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

std::string decToHex(int decimal_value)
{
	std::stringstream ss;
	ss << std::hex << decimal_value; // int decimal_value
	std::string res(ss.str());
	return res;
}
