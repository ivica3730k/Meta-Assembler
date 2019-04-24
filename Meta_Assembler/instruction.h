#include <string>
#include <vector>
class instruction {
public:
	instruction(const std::string& _name, const std::string& _code, int bitc);
	std::string name;
	std::string code;
	int arglen;
	bool isKeyword(std::vector<std::string>* keyw);
};