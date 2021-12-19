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
#include <cmath>
#include <chrono>
#include <ranges>

bool explode_number(std::string& nr) {
	// look for pair nested inside four other pairs -> stack depth 4
	int depth = 0;
	int nested_pair_start = -1;
	int nested_pair_end = -1;
	int idx = 0;
	for (auto c : nr) {
		switch(c) {
			case '[':
				if (depth == 4) {
					nested_pair_start = idx;
				}
				depth++;
				break;
			case ']':
				depth--;
				if (depth == 4) {
					nested_pair_end = idx;
				}
				break;
		}

		if (nested_pair_end > -1) break;

		idx++;
	}

	if (nested_pair_end > -1) {
		std::string nested_pair = nr.substr(nested_pair_start, nested_pair_end - nested_pair_start + 1);
		//printf("nested pair: %s\n", nested_pair.c_str());
		std::istringstream np(nested_pair);
		std::string s;
		getline(np, s, '[');
		getline(np, s, ',');
		int nr1 = std::atoi(s.c_str());
		getline(np, s, ']');
		int nr2 = std::atoi(s.c_str());

		// find right number (if any)
		int right_nr_idx = nested_pair_end + 1;
		while (!isdigit(nr[right_nr_idx])) {
			right_nr_idx++;
			if (right_nr_idx == nr.length()) break;
		}
		if (right_nr_idx < nr.length()) {
			int right_bracket_idx = right_nr_idx;
			while (isdigit(nr[right_bracket_idx])) {
				right_bracket_idx++;
			}
			int nr3 = std::atoi(nr.substr(right_nr_idx, right_bracket_idx - right_nr_idx).c_str());

			// replace with sum nr2 + nr3
			nr.replace(right_nr_idx, right_bracket_idx - right_nr_idx, std::to_string(nr2 + nr3));
		}

		// replace nested pair with 0
		nr.replace(nested_pair_start, nested_pair_end - nested_pair_start + 1, "0");

		// find left comma (if any)
		int left_nr_idx = nested_pair_start - 1;
		while (!isdigit(nr[left_nr_idx])) {
			left_nr_idx--;
			if (left_nr_idx < 0) break;
		}
		if (left_nr_idx > 0) {
			int left_bracket_idx = left_nr_idx;
			while (isdigit(nr[left_bracket_idx])) {
				left_bracket_idx--;
			}

			int nr0 = std::atoi(nr.substr(left_bracket_idx + 1, left_nr_idx - left_bracket_idx).c_str());

			// replace with sum nr0 + nr1
			nr.replace(left_bracket_idx + 1, left_nr_idx - left_bracket_idx, std::to_string(nr0 + nr1));
		}

		return true;
	}

	return false;
}

void explode_number_test(std::string nr) {
	printf("Exploding %s\n", nr.c_str());
	if (explode_number(nr)) {
		printf("Result: %s\n", nr.c_str());
	} else {
		printf("Didn't explode\n");
	}
	printf("\n");
}

bool split_number(std::string& nr) {
	// check for split action
	int current_number = 0;
	int idx = 0;
	int number_start = 0;
	int number_end = 0;
	bool in_number = false;
	bool split_action = false;
	for (auto c : nr) {
		switch(c) {
			case '[':
			case ']':
			case ',':
				if (current_number >= 10) {
					split_action = true;
				}
				current_number = 0;
				in_number = false;
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if (!in_number) {
					number_start = idx;
				} else {
					number_end = idx;
				}
				in_number = true;
				current_number = current_number * 10 + (c - '0');
				break;
		}

		if (split_action) break;

		idx++;
	}

	if (split_action) {
		int nr_to_split = std::atoi(nr.substr(number_start, number_end - number_start + 1).c_str());
		int nr1 = nr_to_split / 2;
		int nr2 = (nr_to_split + 1) / 2;
		auto new_string = std::string("[");
		new_string += std::to_string(nr1);
		new_string += std::string(",");
		new_string += std::to_string(nr2);
		new_string += std::string("]");
		nr.replace(number_start, number_end - number_start + 1, new_string);

		printf("split action: %i => %s\n", nr_to_split, new_string.c_str());
	}

	return split_action;
}

std::string reduce_number(std::string nr) {
	printf("Reducing %s\n", nr.c_str());

	for (;;) {
		bool explode_action = explode_number(nr);
		if (explode_action) {
			printf("after explode: %s\n", nr.c_str());
			continue;
		}

		bool split_action = split_number(nr);
		if (split_action) {
			printf("after split: %s\n", nr.c_str());
		}
		if (!split_action) break;
	}

	return nr;
}

std::string add_numbers(std::string nr1, std::string nr2) {
	return std::string("[") + nr1 + std::string(",") + nr2 + std::string("]");
}

int magnitude(std::string nr, int& idx) {
	// first character is expected to be '['
	int left_magnitude = 0;
	int right_magnitude = 0;

	// check second character
	idx++;
	int start_idx = idx;
	if (nr[idx] == '[') { // recursion
		left_magnitude = magnitude(nr, idx) * 3;
		idx++;
	} else {
		while (isdigit(nr[idx])) {
			idx++;
		}
		left_magnitude = std::atoi(nr.substr(start_idx, idx - start_idx).c_str()) * 3;
	}

	idx++;
	start_idx = idx;
	if (nr[idx] == '[') { // recursion
		right_magnitude = magnitude(nr, idx) * 2;
		idx++;
	} else {
		while (isdigit(nr[idx])) {
			idx++;
		}
		right_magnitude = std::atoi(nr.substr(start_idx, idx - start_idx).c_str()) * 2;
	}

	return left_magnitude + right_magnitude;
}

void magnitude_test(std::string nr) {
	int idx = 0;
	printf("Magnitude of %s: %i\n", nr.c_str(), magnitude(nr, idx));
}

int main() {
	//std::ifstream input("example1.txt");
	//std::ifstream input("example2.txt");
	std::ifstream input("input.txt");

	if (!input.is_open()) {
		printf("Error opening file\n");
		return -1;
	}

	// explode tests
	/*
	explode_number_test(std::string("[1,2]")); // expected: "didn't explode"
	explode_number_test(std::string("[[[[[9,8],1],2],3],4]")); // expected: [[[[0,9],2],3],4]
	explode_number_test(std::string("[7,[6,[5,[4,[3,2]]]]]")); // expected: [7,[6,[5,[7,0]]]]
	explode_number_test(std::string("[[6,[5,[4,[3,2]]]],1]")); // expected: [[6,[5,[7,0]]],3]
	explode_number_test(std::string("[[3,[2,[1,[7,3]]]],[6,[5,[4,[3,2]]]]]")); // expected: [[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]
	explode_number_test(std::string("[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]")); // expected: [[3,[2,[8,0]]],[9,[5,[7,0]]]]
	*/

	// add and reduce test
	//reduce_number(add_numbers("[[[[4,3],4],4],[7,[[8,4],9]]]", "[1,1]"));
	// expected: 
	/*after addition: [[[[[4,3],4],4],[7,[[8,4],9]]],[1,1]]
	  after explode:  [[[[0,7],4],[7,[[8,4],9]]],[1,1]]
	  after explode:  [[[[0,7],4],[15,[0,13]]],[1,1]]
	  after split:    [[[[0,7],4],[[7,8],[0,13]]],[1,1]]
	  after split:    [[[[0,7],4],[[7,8],[0,[6,7]]]],[1,1]]
	  after explode:  [[[[0,7],4],[[7,8],[6,0]]],[8,1]]*/

	// magnitude tests
	/*
	magnitude_test(std::string("[9,1]")); // expected: 29
	magnitude_test(std::string("[[9,1],[1,9]]")); // expected: 129
	magnitude_test(std::string("[[1,2],[[3,4],5]]")); // expected: 143
	magnitude_test(std::string("[[[[0,7],4],[[7,8],[6,0]]],[8,1]]")); // expected: 1384
	magnitude_test(std::string("[[[[1,1],[2,2]],[3,3]],[4,4]]")); // expected: 445
	magnitude_test(std::string("[[[[3,0],[5,3]],[4,4]],[5,5]]")); // expected: 791
	magnitude_test(std::string("[[[[5,0],[7,4]],[5,5]],[6,6]]")); // expected: 1137
	magnitude_test(std::string("[[[[8,7],[7,7]],[[8,6],[7,7]]],[[[0,7],[6,6]],[8,7]]]")); // expected: 3488
	*/

	std::vector<std::string> numbers;

	std::string line;
	while (getline(input, line)) {
		numbers.push_back(line);
	}

    int largest_magnitude = 0;
    for (int nr0 = 0; nr0 < numbers.size(); nr0++) {
        for (int nr1 = 0; nr1 < numbers.size(); nr1++) {
            if (nr0 == nr1) continue;

	        int idx = 0;
            int mg = magnitude(reduce_number(add_numbers(numbers[nr0], numbers[nr1])), idx);
            largest_magnitude = std::max(largest_magnitude, mg);
        }
    }

	printf("largest magnitude: %i\n", largest_magnitude);

	return 0;
}
