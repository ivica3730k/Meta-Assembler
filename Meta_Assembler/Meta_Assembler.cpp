#include "pch.h"
#include "constants.h"
#include "instruction.h"
#include "functions.h"
#include "label.h"

int main(int argc, char** argv)
{

	int bits = 16; //default arhitecture for system is 16 bits
	struct file {
		bool valid = false;
		std::string name = "";
	};

	file codefile;
	file instrfile;

	for (int i = 0; i < argc; ++i) {

		if (strcmp(argv[i], "-f") == 0) {
			++i;
			codefile.name = argv[i];
			codefile.valid = true;
			continue;
		}

		if (strcmp(argv[i], "-bit") == 0) {
			++i;
			bits = std::stoi(argv[i]);
			continue;
		}

		if (strcmp(argv[i], "-d") == 0) {
			++i;
			instrfile.name = argv[i];
			instrfile.valid = true;
			continue;
		}
	}

	std::vector<instruction> instructions;
	std::vector<label> labels;

	if (instrfile.valid == false) {
		instructions.push_back({ "ADDI", "D", bits });
		instructions.push_back({ "JMP", "E", bits });
		instructions.push_back({ "MOVEI", "C", bits });
		instructions.push_back({ "MOVE", "B", bits });
		instructions.push_back({ "STORE", "A", bits });
		instructions.push_back({ "LDD", "9", bits });
		instructions.push_back({ "BZ", "FE", bits });
		instructions.push_back({ "BNZ", "FD", bits });
		instructions.push_back({ "STA", "FFFF", bits });
		instructions.push_back({ "STA+", "FFFE", bits });
	}
	else {
		std::ifstream instructionFile(instrfile.name);
		if (instructionFile.is_open()) {
			std::string line;
			unsigned long int linen = 0;
			while (getline(instructionFile, line)) {
				std::transform(line.begin(), line.end(), line.begin(), ::toupper);

				if (line == "") {

					// skip blank lines
					linen++;
					continue;
				}
				if (line[0] == '*') {
					linen++;
					continue;
				}

				std::vector<std::string> results;
				std::istringstream iss(line);
				std::string token;

				int a = 0;
				while (getline(iss, token, '\t')) { // but we can specify a different one
					results.push_back(token);
				}
				if (isInputKeyword(results[0], &keywords) == false) {
					instruction cacheInstr(results[0], results[1], bits);
					instructions.push_back(cacheInstr);
				}
				else {
					std::cout << "Error in instruction file, line " << linen << " ." << results[0] << " is reserved ASM word!" << std::endl;
					return 0;
				}

				linen++;
			}
			instructionFile.close();
		}
		else {
			std::cout << "Unable to open instruction file!" << std::endl;
			return 0;
		}
	}

	if (codefile.valid == false) {
		//throw std::runtime_error("Error, no codefile specified. Specify codefile using -f 'filename'");
		std::cout << "Error, no codefile specified. Specify codefile using -f 'filename'" << std::endl;
		return 0;
	}

	//----------if codefile is valid read codefile line by line-----------
	std::ifstream myfile(codefile.name);
	if (myfile.is_open()) {
		unsigned long int pc = 0;
		unsigned long int pcmax = LONG_MAX;
		unsigned long int linenum = 0;
		std::vector<std::string> cdms;
		std::string line;
		bool isInLabel = false;
		while (getline(myfile, line)) {
			std::transform(line.begin(), line.end(), line.begin(), ::toupper);

			if (line == "") {
				std::cout << "isblank";
				linenum++;
				continue;
			}
			if (line[0] == '*') {
				std::cout << "iscomment";
				linenum++;
				continue;
			}

			if (line.back() == ':') {
				std::cout << "islabel";
				line = line.substr(0, line.size() - 1);
				if (isInputKeyword(line, &keywords)) {
					std::cout << "Error on line " << linenum << " :";
					std::cout << "Label " << line << " is reserved assembler keyword" << std::endl;
					return (0);
				}
				label cachel;
				cachel.name = line;
				cachel.startadr = pc;
				labels.push_back(cachel);
				std::cout << "found label";
				continue;
			}

			bool hasData = false;

			std::vector<std::string> results;
			std::istringstream iss(line);
			std::string token;
			int a = 0;
			while (getline(iss, token, '\t')) { // but we can specify a different one
				results.push_back(token);
				a++;
			}
			if (a > 1) {
				hasData = true;
			}

			std::cout << hasData;

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
			for (i = instructions.begin(); i < instructions.end(); ++i) {
				instruction ins = *i;

				if (ins.name == results[0]) {
					//std::cout << pc << std::endl;
					if (hasData == false) {
						std::string cache = decToHex(pc, bits);
						cache += ":";
						cache += ins.code;
						cdms.push_back(cache);
					}

					else {

						bool isLabel = false;
						label cacheLabel;
						cacheLabel.name = results[1];

						if (std::find(labels.begin(), labels.end(), cacheLabel) != labels.end()) {
							isLabel = true;
							std::vector<label>::iterator i;
							for (i = labels.begin(); i < labels.end(); ++i) {
								label l;
								l = *i;
								if (l == cacheLabel) {
									cacheLabel = l;
								}
							}
						}
						
						std::cout << "islbl";
						std::cout << isLabel << std::endl;

						if (isLabel == false) {

							if (results[1].length() != ins.arglen) {
								std::cout << "Error on line " << linenum << " :";
								std::cout << "Operation " << ins.name << " can only be used with " << ins.arglen << " byte argument!" << std::endl;
								return (0);
							}
							std::cout << "res are";
							std::cout << results[1] << std::endl;
							std::string cache = decToHex(pc, bits);
							cache += ":";
							cache += ins.code;
							cache += results[1];
							cdms.push_back(cache);
						}

						else {
							std::cout << "in label mode";
							std::string cache = decToHex(pc, bits);
							cache += ":";
							cache += ins.code;
							cache += decToHex(cacheLabel.startadr, bits);
							cdms.push_back(cache);
						}
					}

					pc++;
				}

				isMnemonicValid = true;
			}

			if (isMnemonicValid == false) {
				std::cout << "Error on line " << linenum << ": ";
				std::cout << "Operation " << results[0] << " not valid operation, aborting!" << std::endl;
				return 0;
			}
			linenum++;
		}
		myfile.close();

		if (pc > pcmax) {
			std::cout << "Size of the code is too big for addres space you have chosen, please increase your address space" << std::endl;
			return 0;
		}

		size_t lastindex = codefile.name.find_last_of(".");
		codefile.name = codefile.name.substr(0, lastindex);
		codefile.name += ".cdm";
		std::ofstream myfile(codefile.name);

		if (myfile.is_open()) {
			std::vector<std::string>::iterator i;
			for (i = cdms.begin(); i < cdms.end(); ++i) {
				std::string cache = *i;
				myfile << cache << std::endl;
			}
		}
	}

	else {
		std::cout << "Unable to open codefile";
		return 0;
	}
	std::cout << "CDM file created succesfully" << std::endl;
	return 1;
}
