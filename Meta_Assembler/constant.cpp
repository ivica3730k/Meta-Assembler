#include "pch.h"
#include "constant.h"


constant::constant()
{
}

constant::constant(std::string _name, std::string _value)
{
}

unsigned long constant::returnValueAsLong()
{
	return std::stoi(value);
}

std::string constant::returnValueAsString()
{
	return name;
}

bool constant::operator==(const constant & p) const
{
	return name == p.name;
}


