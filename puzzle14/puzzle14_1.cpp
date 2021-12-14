#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <array>
#include <stack>
#include <map>
#include <set>
#include <algorithm>

int main() {
	//std::ifstream input("example.txt");
	std::ifstream input("input.txt");

	if (!input.is_open()) {
		printf("Error opening file\n");
		return -1;
	}

	std::string current_polymer, next_polymer;
	std::map<std::string, char> rules;

	std::string line;
	// polymer template
	getline(input, line);
	current_polymer = line;
	// empty line
	getline(input, line);
	// pair insertion rules
	while (getline(input, line)) {
		std::string s = std::string("") + line[0] + line[1];
		rules.insert(std::pair<std::string, char>(s, line[line.length() - 1]));
	}

	// debug: print rules
	/*printf("rules:\n");
	for (auto r : rules) {
		printf("%s -> %c\n", r.first.c_str(), r.second);
	}
	printf("\n");*/

	printf("Template: %s\n", current_polymer.c_str());
	const int num_steps = 10;
	for (int i = 0; i < num_steps; i++) {
		next_polymer = std::string("");
		char c1 = current_polymer[0];
		char c2 = current_polymer[1];
		for (int j = 0; j < current_polymer.length() - 1; j++) {
			std::string both = std::string("") + c1 + c2;
			next_polymer += c1;
			next_polymer += rules[both];

			if (j < current_polymer.length() - 2) {
				c1 = c2;
				c2 = current_polymer[j + 2];
			}
		}
		next_polymer += current_polymer[current_polymer.length() - 1];

		//printf("After step %i: %s\n", i + 1, next_polymer.c_str());

		current_polymer = next_polymer;
	}

	int most_common_count = -1, least_common_count = current_polymer.length() + 1;

	std::set<char> different_characters(current_polymer.begin(), current_polymer.end());
	for (auto c: different_characters) {
		int count = std::count(current_polymer.begin(), current_polymer.end(), c);
		least_common_count = std::min(least_common_count, count);
		most_common_count = std::max(most_common_count, count);
	}


	std::cout << "Most common after " << num_steps << " steps: " << most_common_count << "\n";
	std::cout << "Least common after " << num_steps << " steps: " << least_common_count << "\n";
	std::cout << "Result: " << most_common_count - least_common_count << "\n";

	return 0;
}
