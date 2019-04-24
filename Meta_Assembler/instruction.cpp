#include "pch.h"
#include "instruction.h"



instruction::instruction(const std::string& _name, const std::string& _code, int bitc)
{
	name = _name;
	code = _code;
	arglen = (bitc / 4) - code.length();
}

bool instruction::isKeyword(std::vector<std::string>* keyw)
{
	std::vector<std::string>::iterator i;
	for (i = keyw->begin(); i < keyw->end(); ++i) {
		std::string cache = *i;
		if (strcmp(cache.c_str(), name.c_str())) {
			return false;
		}
	}
	return true;
}