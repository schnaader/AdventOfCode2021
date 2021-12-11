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

	int h = 0, d = 0, a = 0;

	std::string line;
	while (getline(input, line)) {
		std::istringstream ls(line);
		std::string direction;
		std::string value;
		getline(ls, direction, ' ');
		getline(ls, value, ' ');
		int value_int = atoi(value.c_str());
		//printf("%i\n", value_int);
		if (direction == "forward") {
			h += value_int;
			d += a * value_int;
		} else if (direction == "down") {
			a += value_int;
		} else if (direction == "up") {
			a -= value_int;
		}
	}
	input.close();

	printf("h: %i, d: %i, product: %i\n", h, d, h * d);

	return 0;
}
