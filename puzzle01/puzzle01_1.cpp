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

	int increase_count = 0;

	std::string line;
	int previous_one = -1;
	while (getline(input, line)) {
		int this_one = atoi(line.c_str());
		if ((this_one > previous_one) && (previous_one > -1)) {
			increase_count++;
		}
		previous_one = this_one;
	}
	input.close();

	printf("increase count: %i\n", increase_count);

	return 0;
}
