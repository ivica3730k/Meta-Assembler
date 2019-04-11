#include "pch.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>

class mnemonic {
public:
	mnemonic(const std::string& _name, const std::string& _code);
	std::string name;
	std::string code;
	int arglen;
};

mnemonic::mnemonic(const std::string& _name, const std::string& _code)
{
	name = _name;
	code = _code;
	arglen = 4 - code.length();
}

mnemonic mnemonics[10] = {

	{ "ADDI", "D" },
	{ "JMP", "E" },
	{ "MOVEI", "C" },
	{ "MOVE", "B" },
	{ "STORE", "A" },
	{ "LDD", "9" },
	{ "BZ", "FE" },
	{ "BNZ", "FD" },
	{ "STA", "FFFF" },
	{ "STA+", "FFFE" }

};

std::string decToHex(int decimal_value) {
	std::stringstream ss;
	ss << std::hex << decimal_value; // int decimal_value
	std::string res(ss.str());
	return res;
}

int main(int argc, char** argv)
{

	//----------open file that is passed with -f argument----------
	struct file {
		bool valid = false;
		std::string name = "";
	};

	file file;

	for (int i = 0; i < argc; ++i) {

		if (strcmp(argv[i], "-f") == 0) {
			++i;
			std::cout << "Filename that you want to read is: " << argv[i] << std::endl;
			file.name = argv[i];
			file.valid = true;
		}
	}

	if (file.valid == false) {
		//throw std::runtime_error("Error, no file specified. Specify file using -f 'filename'");
		std::cout << "Error, no file specified. Specify file using -f 'filename'" << std::endl;
		return 0;
	}

	//----------if file is valid read file line by line-----------
	std::ifstream myfile(file.name);
	if (myfile.is_open()) {
		unsigned long int pc = 0;
		unsigned long int linenum = 0;
		std::vector<std::string>cdms;
		std::string line;
		while (getline(myfile, line)) {
			linenum++;
			if (line == "")
				continue;
			int spaces = std::count(line.begin(), line.end(), ' ');
			
			std::vector<std::string>results;
			if (spaces != 0) {
				std::istringstream iss(line);
				std::vector<std::string> filter_results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
				results.push_back(filter_results[0]);
				results.push_back(filter_results[1]);
			}
			else {
				results.push_back(line);
				results.push_back("");
			}
	
		
			bool isMnemonicValid = false;
			for (int i = 0; i < 10; i++) {
				if (mnemonics[i].name == results[0]) {

					if (results[1].length() != mnemonics[i].arglen) {
						std::cout << "Error on line " << linenum << " :";
						std::cout << "Operation " << mnemonics[i].name << " can only be used with " << mnemonics[i].arglen << " byte argument!" << std::endl;
						return (0);
					}

					std::cout <<decToHex(pc)<<":"<< mnemonics[i].code << std::endl;
					pc++;
					isMnemonicValid = true;
					break;
				}
			}

			if (isMnemonicValid == false) {
				std::cout << "Error on line " << linenum << ": ";
				std::cout << "Operation " << results[0] << " not on list of valid operations, aborting!" << std::endl;
				return 0;
			}
		}
		myfile.close();
	}

	else {
		std::cout << "Unable to open file";
		return 0;
	}

	return 1;
}
