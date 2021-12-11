#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>

int sum_up(int n) {
	int sum = 0;
	for (int i = 1; i <= n; i++) {
		sum += i;
	}
	return sum;
}

int main() {
	int sum_unmarked = 0;

	//std::ifstream input("example.txt");
	std::ifstream input("input.txt");

	if (!input.is_open()) {
		printf("Error opening file\n");
		return -1;
	}

	std::vector<int> crab_positions;
	std::string first_line;
	getline(input, first_line);
	std::istringstream fl(first_line);
	std::string number;
	while (getline(fl, number, ',')) {
		crab_positions.push_back(atoi(number.c_str()));
	}
	printf("Crab count: %li\n", crab_positions.size());

	int min_fuel;
	bool first_one = true;
	for (int align_pos = 0; align_pos < crab_positions.size(); align_pos++) {
		int fuel = 0;
		for (int i = 0; i < crab_positions.size(); i++) {
				fuel += sum_up(std::abs(crab_positions[i] - align_pos));
				//if (align_pos == 2) printf("%i ", std::abs(crab_positions[i] - align_pos));
		}
		//if (align_pos == 2) printf("\n");
		
		if (first_one) {
			min_fuel = fuel;
			first_one = false;
		} else {
			min_fuel = std::min(fuel, min_fuel);
		}
		
		/*if (align_pos == 2) {
			printf("Fuel at align pos 2: %i\n", fuel);
		}*/
	}
	
	printf("Minimal fuel: %i\n", min_fuel);
}
