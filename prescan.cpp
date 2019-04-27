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
