#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>

int main() {
	int sum_unmarked = 0;

	//std::ifstream input("example.txt");
	std::ifstream input("input.txt");

	if (!input.is_open()) {
		printf("Error opening file\n");
		return -1;
	}

	std::array<int64_t, 9> fish_timer_count;
	for (int i = 0; i < 9; i++) {
		fish_timer_count[i] = 0;
	}
	std::string first_line;
	getline(input, first_line);
	std::istringstream fl(first_line);
	std::string number;
	while (getline(fl, number, ',')) {
		fish_timer_count[atoi(number.c_str())]++;
	}
	int64_t fish_count = 0;
	for (int i = 0; i < 9; i++) {
		fish_count += fish_timer_count[i];
	}
	printf("Fish count: %li\n", fish_count);

	for (int i = 0; i < 256; i++) {
		std::array<int64_t, 9> new_count;
		
		for (int i = 1; i < 9; i++) {
			new_count[i - 1] = fish_timer_count[i];
		}
		new_count[8] = fish_timer_count[0];
		new_count[6] += fish_timer_count[0];
		
		fish_timer_count = new_count;
	}

	fish_count = 0;
	for (int i = 0; i < 9; i++) {
		fish_count += fish_timer_count[i];
	}
	printf("Fish count after 256 days: %li\n", fish_count);
}
