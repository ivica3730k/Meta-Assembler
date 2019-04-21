#include "pch.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>

//*********************************************CLASS FOR INSTRUCTION STORING****************************//
class instruction {
public:
	instruction(const std::string& _name, const std::string& _code, int bitc);
	std::string name;
	std::string code;
	int arglen;
	bool isKeyword(std::vector<std::string>* keyw);
};

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

//*****************************************************************************************************//

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
/*
bool isInputKeyword(const char* input, std::vector<std::string>* keyw) {
	std::vector<std::string>::iterator i;
	for (i = keyw->begin(); i < keyw->end(); ++i) {
		std::string cache = *i;
		if (strcmp(cache.c_str(), input)) {
			return true;
		}
	}
	return false;
}
*/

std::string decToHex(int decimal_value)
{
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
			file.name = argv[i];
			file.valid = true;
			continue;
		}

		if (strcmp(argv[i], "-bit") == 0) {
			++i;
			bits = std::stoi(argv[i]);
			continue;
		}
	}
	/*
	instruction instructions[10] = {

		{ "ADDI", "D", bits },
		{ "JMP", "E", bits },
		{ "MOVEI", "C", bits },
		{ "MOVE", "B", bits },
		{ "STORE", "A", bits },
		{ "LDD", "9", bits },
		{ "BZ", "FE", bits },
		{ "BNZ", "FD", bits },
		{ "STA", "FFFF", bits },
		{ "STA+", "FFFE", bits }

	};
	*/
	std::vector<instruction> instrs;
	instrs.push_back({ "ADDI", "D", bits });
	instrs.push_back({ "JMP", "E", bits });
	instrs.push_back({ "MOVEI", "C", bits });
	instrs.push_back({ "MOVE", "B", bits });
	instrs.push_back({ "STORE", "A", bits });
	instrs.push_back({ "LDD", "9", bits });
	instrs.push_back({ "BZ", "FE", bits });
	instrs.push_back({ "BNZ", "FD", bits });
	instrs.push_back({ "STA", "FFFF", bits });
	instrs.push_back({ "STA+", "FFFE", bits });

	std::vector<std::string> keywords = { "$", "DF", "GROUP", "ORG", "*", "DGROUP", "GT", "%OUT", "+",
		"DOSSEG", "HIGH", "PAGE", "_", "DQ", "IF", "PARA,", ".", "DS", "IF1", "PROC", "/", "DT",
		"IF2", "PTR", "=", "DUP", "IFB", "PUBLIC", "?", "DW", "IFDEF", "PURGE", "DWORD",
		"IFGIF", "QWORD", ".186", "ELSE", "IFDE", ".RADIX", ".286", "END", "IFIDN",
		"RECORD", ".286P", "ENDIF", "IFNB", "REPT", ".287", "ENDM", "IFNDEF", ".SALL", ".386",
		"ENDP", "INCLUDE", "SEG", ".386P", "ENDS", "INCLUDELIB", "SEGMENT",
		".387", "EQ", "IRP", ".SEQ", ".8086", "EQU", "IRPC", ".SFCOND", ".8087", ".ERR",
		"LABEL", "SHL", "ALIGN", ".ERR1", ".LALL", "SHORT", ".ALPHA", ".ERR2", "LARGE",
		"SHR", "AND", ".ERRB", "LE", "SIZE", "ASSUME", ".ERRDEF", "LENGTH", "SMALL", "AT",
		".ERRDIF", ".LFCOND", "STACK", "BYTE", ".ERRE", ".LIST", "@STACK", ".CODE",
		".ERRIDN", "LOCAL", ".STACK", "@CODE", ".ERRNB", "LOW", "STRUC", "@CODESIZE",
		".ERRNDEF", "LT", "SUBTTL", "COMM", ".ERRNZ", "MACRO", "TBYTE", "COMMENT", "EVEN",
		"MASK", ".TFCOND", ".CONST", "EXITM", "MEDIUM", "THIS", ".CREF", "EXTRN", "MOD", "TITLE",
		"@CURSEG", "FAR", ".MODEL", "TYPE", "@DATA", "@FARDATA", "NAME", ".TYPE", ".DATA", ".FARDATA",
		"NE", "WIDTH", "@DATA?", "@FARDATA?", "NEAR", "WORD", ".DATA",
		"?", ".FARDATA ?", "NOT", "@WORDSIZE", "@DATASIZE", "@FILENAME", "NOTHING",
		".XALL", "DB", "FWORD", "OFFSET", ".XCREP", "DD", "GE", "OR", ".XLIST", "XOR" };

	if (file.valid == false) {
		//throw std::runtime_error("Error, no file specified. Specify file using -f 'filename'");
		std::cout << "Error, no file specified. Specify file using -f 'filename'" << std::endl;
		return 0;
	}

	//----------if file is valid read file line by line-----------
	std::ifstream myfile(file.name);
	if (myfile.is_open()) {
		unsigned long int pc = 0;
		unsigned long int pcmax = INT_MAX;
		unsigned long int linenum = 0;
		std::vector<std::string> cdms;
		std::string line;
		while (getline(myfile, line)) {
			std::transform(line.begin(), line.end(), line.begin(), ::toupper);
			linenum++;
			if (line == "")
				continue;

			bool hasData = false;

			std::vector<std::string> results;

			std::istringstream iss(line);
			std::string token;
			int a = 0;
			while (getline(iss, token, '\t')) { // but we can specify a different one
				results.push_back(token);
				a++;
			}
			if (a > 1)
				hasData = true;

			/*Check here for static ASM tags like ORG,END...*/
			if (isInputKeyword(results[0], &keywords)) {

				if (results[0] == "ORG") {
					std::string str = results[1];
					str.erase(std::remove(str.begin(), str.end(), '$'), str.end());
					pc = stoi(str);
				}
				if (results[0] == "END") {
					std::string str = results[1];
					str.erase(std::remove(str.begin(), str.end(), '$'), str.end());
					pcmax = stoi(str);
				}
				continue;
			}

			bool isMnemonicValid = false;
			std::vector<instruction>::iterator i;
			for (i = instrs.begin(); i < instrs.end(); ++i) {
				instruction ins = *i;

				if (ins.name == results[0]) {

					if (ins.isKeyword(&keywords)) {
						std::cout << "Error on line " << linenum << " :";
						std::cout << "Operation " << ins.name << " is reserved assembler keyword" << std::endl;
					}

					if (results[1].length() != ins.arglen) {
						std::cout << "Error on line " << linenum << " :";
						std::cout << "Operation " << ins.name << " can only be used with " << ins.arglen << " byte argument!" << std::endl;
						return (0);
					}

					if (hasData == false) {
						std::string cache = decToHex(pc);
						cache += ":";
						cache += ins.code;
						cdms.push_back(cache);
					}
					else {
						std::string cache = decToHex(pc);
						cache += ":";
						cache += ins.code;
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

		if (pc > pcmax) {
			std::cout << "Size of the code is too big for addres space you have chosen, please increase your address space" << std::endl;
			return 0;
		}

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
