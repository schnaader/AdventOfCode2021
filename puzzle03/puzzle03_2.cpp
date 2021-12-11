#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

int w;
std::vector<std::string> lines;

int get_oxygen_rating(int x, std::vector<std::string> numbers) {
	int most_common_bit = 0;
	int count0 = 0, count1 = 0;
	for (int y = 0; y < numbers.size(); y++) {
		if (numbers[y][x] == '0') {
			count0++;
		} else {
			count1++;
		}
	}
	if (count1 >= count0) {
		most_common_bit = 1;
	}
	//printf("size before: %li\n", numbers.size());
	//for (auto s: numbers) {
	//	printf("%s\n", s.c_str());
	//}

	std::erase_if(numbers, [x, most_common_bit](std::string s) {
			return (s[x] != (most_common_bit + '0'));
		});

	//printf("size after: %li\n", numbers.size());

	if (numbers.size() == 1) {
		//printf("Only one left: %s\n", numbers[0].c_str());
		return std::stoi(numbers[0], 0, 2);
	}

	if ((x + 1) < w) {
		return get_oxygen_rating(x + 1, numbers);
	}

	exit(-2);
}

int get_co2_rating(int x, std::vector<std::string> numbers) {
	int least_common_bit = 0;
	int count0 = 0, count1 = 0;
	for (int y = 0; y < numbers.size(); y++) {
		if (numbers[y][x] == '0') {
			count0++;
		} else {
			count1++;
		}
	}
	if (count1 < count0) {
		least_common_bit = 1;
	}
	std::erase_if(numbers, [x, least_common_bit](std::string s) {
			return (s[x] != (least_common_bit + '0'));
		});

	if (numbers.size() == 1) {
		//printf("Only one left: %s\n", numbers[0].c_str());
		return std::stoi(numbers[0], 0, 2);
	}

	if ((x + 1) < w) {
		return get_co2_rating(x + 1, numbers);
	}

	exit(-3);
}

int main() {
	//std::ifstream input("example.txt");
	std::ifstream input("input.txt");

	if (!input.is_open()) {
		printf("Error opening file\n");
		return -1;
	}

	std::string line;
	while (getline(input, line)) {
		lines.push_back(line);
	}
	input.close();
	// oxygen generator rating

	w = lines[0].length();
	int oxygen = get_oxygen_rating(0, lines);
	int co2 = get_co2_rating(0, lines);

	printf("oxygen: %i, co2: %i, product: %i\n", oxygen, co2, oxygen * co2);

	return 0;
}
