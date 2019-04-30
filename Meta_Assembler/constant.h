#pragma once
class constant
{
public:
	constant();
	constant(std::string _name,std::string _value);
	unsigned long returnValueAsLong();
	std::string returnValueAsString();
	bool operator==(const constant& p) const;
	std::string name;
	std::string value;
};

