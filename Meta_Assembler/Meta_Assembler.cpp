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
	mnemonic(const std::string& _name, const std::string& _code,int bitc);
	std::string name;
	std::string code;
	int arglen;
	
};

mnemonic::mnemonic(const std::string& _name, const std::string& _code,int bitc)
{
	name = _name;
	code = _code;
	arglen = (bitc/4) - code.length();
}

std::string decToHex(int decimal_value) {
	std::stringstream ss;
	ss << std::hex << decimal_value; // int decimal_value
	std::string res(ss.str());
	return res;
}

int main(int argc, char** argv)
{
	int bits = 16; //default arhitecture for system is 16 bits
	//----------open file that is passed with -f argument----------
	struct file {
		bool valid = false;
		std::string name = "";
	};
	file file;
	for (int i = 0; i < argc; ++i) {

		if (strcmp(argv[i], "-f") == 0) {
			++i;
			//std::cout << "Filename that you want to read is: " << argv[i] << std::endl;
			file.name = argv[i];
			file.valid = true;
			continue;
		}

		if (strcmp(argv[i], "-bit") == 0) {
			++i;
			//std::cout << "Filename that you want to read is: " << argv[i] << std::endl;
			bits = std::stoi(argv[i]);
			//std::cout << bitN;
			continue;
		}
	}

	mnemonic mnemonics[10] = {

	{ "ADDI", "D",bits },
	{ "JMP", "E",bits },
	{ "MOVEI", "C",bits },
	{ "MOVE", "B" ,bits},
	{ "STORE", "A",bits },
	{ "LDD", "9",bits },
	{ "BZ", "FE",bits },
	{ "BNZ", "FD",bits},
	{ "STA", "FFFF",bits },
	{ "STA+", "FFFE",bits }

	};

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
			std::transform(line.begin(), line.end(), line.begin(), ::toupper);
			linenum++;
			if (line == "")
				continue;
			int spaces = std::count(line.begin(), line.end(), ' ');
			bool hasData = false;
			std::vector<std::string>results;
			if (spaces != 0) {
				std::istringstream iss(line);
				std::vector<std::string> filter_results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
				results.push_back(filter_results[0]);
				results.push_back(filter_results[1]);
				hasData = true;
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

					if (hasData == false) {
						std::string cache = decToHex(pc);
						cache += ":";
						cache += mnemonics[i].code;
						cdms.push_back(cache);
					}
					else {
						std::string cache = decToHex(pc);
						cache += ":";
						cache += mnemonics[i].code;
						cache += results[1];
						cdms.push_back(cache);
					}
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
		size_t lastindex = file.name.find_last_of(".");
		file.name = file.name.substr(0, lastindex);
		file.name += ".cdm";	
		std::ofstream myfile(file.name);

		if (myfile.is_open()) {
			std::vector<std::string>::iterator i;
			for (i = cdms.begin(); i < cdms.end(); ++i) {
				std::string cache = *i;
				myfile << cache << std::endl;
			}
		}
	}

	else {
		std::cout << "Unable to open file";
		return 0;
	}

	return 1;
}
