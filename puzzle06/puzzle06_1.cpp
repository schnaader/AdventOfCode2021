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

	std::vector<int> fish_timers;
	std::string first_line;
	getline(input, first_line);
	std::istringstream fl(first_line);
	std::string number;
	while (getline(fl, number, ',')) {
		fish_timers.push_back(atoi(number.c_str()));
	}
	printf("Fish count: %li\n", fish_timers.size());

	for (int i = 0; i < 80; i++) {
		std::vector<int> new_timers;
		
		int spawn_count = 0;
		int new_timer;
		for (auto timer: fish_timers) {
			if (timer > 0) {
				new_timer = timer - 1;
			} else if (timer == 0) {
				new_timer = 6;
				spawn_count++;
			}
			
			new_timers.push_back(new_timer);
		}
		
		for (int i = 0; i < spawn_count; i++) {
			new_timers.push_back(8);
		}
		
		fish_timers = new_timers;
	}

	printf("Fish count after 80 days: %li\n", fish_timers.size());
}
