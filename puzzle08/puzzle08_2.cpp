#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <array>
#include <vector>
#include <set>

char input_to_real[256];

void sortString(std::string& stringToSort) {
	std::sort(stringToSort.begin(), stringToSort.end());
}

std::array<std::string, 10> ten_patterns;

std::vector<std::string> getPatternsOfLength(size_t length) {
	std::vector<std::string> result;

	for (auto str: ten_patterns) {
		if (str.length() == length) {
			result.push_back(str);
		}
	}

	return result;
}

std::string removeCommonChars(std::string s1, std::string s2) {
	std::set<char> set(s1.begin(), s1.end());
	for (auto c: s2) {
		set.erase(c);
	}

	return std::string(set.begin(), set.end());
}

int segmentsToNumber(std::string segments) {
	if (segments == "cf") {
		return 1;
	} else if (segments == "acf") {
		return 7;
	} else if (segments == "bcdf") {
		return 4;
	} else if (segments == "acdeg") {
		return 2;
	} else if (segments == "acdfg") {
		return 3;
	} else if (segments == "abdfg") {
		return 5;
	} else if (segments == "abcefg") {
		return 0;
	} else if (segments == "abdefg") {
		return 6;
	} else if (segments == "abcdfg") {
		return 9;
	} else if (segments == "abcdefg") {
		return 8;
	}

	printf("Error: Invalid segment %s\n", segments.c_str());
	exit(-1);
}

int main() {
	//std::ifstream input("example.txt");
	std::ifstream input("input.txt");

	if (!input.is_open()) {
		printf("Error opening file\n");
		return -1;
	}

	int64_t sum = 0;

	std::string line;
	while (getline(input, line)) {
		auto found = line.find("|");
		if (found == std::string::npos) {
			printf("Error: Didn't find |\n");
			exit(-2);
		}

		std::string patterns = line;
		printf("Line before: <%s>\n", line.c_str());
		patterns.erase(patterns.begin() + found, patterns.end());
		printf("line after: <%s>\n", line.c_str());
		std::cout << patterns << "\n";
		std::istringstream ps(patterns);
		std::string part;
		int part_count = 0;
		while (getline(ps, part, ' ')) {
			ten_patterns[part_count] = part;
			//printf("%i: <%s>\n", part_count, ten_patterns[part_count].c_str());
			part_count++;
		}
		std::string eg = "abcdefg";

		auto patternL2 = getPatternsOfLength(2)[0];
		auto patternL3 = getPatternsOfLength(3)[0];
		//printf("Pattern length 2: %s, length 3: %s\n", patternL2.c_str(), patternL3.c_str());
		std::string uncommon = removeCommonChars(patternL3, patternL2);
		//printf("Uncommon character: %s\n", uncommon.c_str());
		input_to_real[uncommon[0]] = 'a';
		eg = removeCommonChars(eg, uncommon);

		auto patternsL6 = getPatternsOfLength(6);
		char l2_0 = patternL2[0];
		char l2_1 = patternL2[1];
		bool found_in_all = true;
		for (auto pattern: patternsL6) {
			if (pattern.find(l2_0) == std::string::npos) {
				found_in_all = false;
			}

			if (!found_in_all) break;
		}
		//printf("\nFound in all: ");
		if (found_in_all) {
			//printf("%c\n", l2_0);
			input_to_real[l2_0] = 'f';
			input_to_real[l2_1] = 'c';
		} else {
			//printf("%c\n", l2_1);
			input_to_real[l2_1] = 'f';
			input_to_real[l2_0] = 'c';
		}
		eg = removeCommonChars(eg, patternL2);

		auto patternL4 = getPatternsOfLength(4)[0];
		//printf("patternL4 before remove: %s\n", patternL4.c_str());
		uncommon = removeCommonChars(patternL4, patternL2);
		//printf("patternL4 after remove: %s\n", uncommon.c_str());
		auto patternsL5 = getPatternsOfLength(5);
		char uc_0 = uncommon[0];
		char uc_1 = uncommon[1];
		found_in_all = true;
		for (auto pattern: patternsL5) {
			if (pattern.find(uc_0) == std::string::npos) {
				found_in_all = false;
			}

			if (!found_in_all) break;
		}
		if (found_in_all) {
			input_to_real[uc_0] = 'd';
			input_to_real[uc_1] = 'b';
		} else {
			input_to_real[uc_1] = 'd';
			input_to_real[uc_0] = 'b';
		}
		eg = removeCommonChars(eg, uncommon);

		char eg_0 = eg[0];
		char eg_1 = eg[1];
		found_in_all = true;
		for (auto pattern: patternsL5) {
			if (pattern.find(eg_0) == std::string::npos) {
				found_in_all = false;
			}

			if (!found_in_all) break;
		}

		if (found_in_all) {
			input_to_real[eg_0] = 'g';
			input_to_real[eg_1] = 'e';
		} else {
			input_to_real[eg_1] = 'g';
			input_to_real[eg_0] = 'e';
		}

		/*printf("Mapping input_to_real:\n");
		for (char c = 'a'; c <= 'g'; c++) {
			printf("%c input -> %c real\n", c, input_to_real[c]);
		}
		printf("\n");*/

		// Output values

		int decoded_value = 0;
		printf("Line before: <%s>\n", line.c_str());
		line.erase(0, found + 2);
		printf("Line after: <%s>\n", line.c_str());
		//std::cout << line << "\n";
		std::istringstream ls(line);
		while (getline(ls, part, ' ')) {
			printf("<%s> -> ", part.c_str());
			std::string mapped = "";
			for (auto p: part) {
				mapped = mapped + input_to_real[p];
			}
			printf("<%s> -> ", mapped.c_str());
			sortString(mapped);
			printf("<%s> -> ", mapped.c_str());
			int n = segmentsToNumber(mapped);
			printf("%i\n", n);
			decoded_value = decoded_value * 10 + n;
		}

		sum += decoded_value;
	}
	input.close();

	std::cout << "Sum: " << sum << "\n";

	return 0;
}
