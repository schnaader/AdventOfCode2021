#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

int main() {
	std::ifstream input("input.txt");

	if (!input.is_open()) {
		printf("Error opening file\n");
		return -1;
	}

	int count = 0;

	std::string line;
	while (getline(input, line)) {
		auto found = line.find("|");
		if (found != std::string::npos) {
			line.erase(0, found + 1);
			//std::cout << line << "\n";
			std::istringstream ls(line);
			std::string part;
			while (getline(ls, part, ' ')) {
				switch (part.length()) {
					case 2:
					case 3:
					case 4:
					case 7:
						count++;
						break;
					default:
						break;
				}
			}
		}
	}
	input.close();

	std::cout << "Count: " << count << "\n";

	return 0;
}
