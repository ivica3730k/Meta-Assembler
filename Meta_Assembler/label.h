#pragma once
#include "pch.h"

class label
{
public:
	label(std::string _name, unsigned long int _startadr);
	label();
	unsigned long int startadr = 0;
	std::string name;
	bool operator==(const label& p) const;
};

