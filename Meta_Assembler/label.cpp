#include "pch.h"
#include "label.h"


label::label(std::string _name, unsigned long int _startadr)
{
	name = _name;
	startadr = _startadr;
	
}

label::label()
{
}

bool label::operator==(const label & p) const
{
	return name == p.name;
}




