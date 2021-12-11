#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

int main() {
	std::ifstream input("input.txt");

	if (!input.is_open()) {
		printf("Error opening file\n");
		return -1;
	}

	int gamma = 0, epsilon = 0;

	int w = 0;
	std::vector<std::string> lines;
	std::string line;
	while (getline(input, line)) {
		lines.push_back(line);
	}
	input.close();
	for (int x = 0; x < lines[0].length(); x++) {
		int most_common_bit = 0;
		int count0 = 0, count1 = 0;
		for (int y = 0; y < lines.size(); y++) {
			if (lines[y][x] == '0') {
				count0++;
			} else {
				count1++;
			}
		}
		if (count1 > count0) {
			most_common_bit = 1;
		}

		gamma = (gamma << 1) + most_common_bit;
		epsilon = (epsilon << 1) + (1 - most_common_bit);
	}

	printf("gamma: %i, epsilon: %i, product: %i\n", gamma, epsilon, gamma * epsilon);

	return 0;
}
