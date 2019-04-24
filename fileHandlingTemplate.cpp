if (myfile.is_open()) {
		std::string line;
		while (getline(myfile, line)) {
			std::transform(line.begin(), line.end(), line.begin(), ::toupper);

			if (line == "") // skip blank lines
				continue;

			std::vector<std::string> results;
			std::istringstream iss(line);
			std::string token;
			int a = 0;
			while (getline(iss, token, '\t')) { // but we can specify a different one
				results.push_back(token);
			}
			
			
		}
		myfile.close();