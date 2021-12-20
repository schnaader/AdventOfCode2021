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

int main() {
	//std::ifstream input("example.txt");
	std::ifstream input("input.txt");

	if (!input.is_open()) {
		printf("Error opening file\n");
		return -1;
	}

	std::array<int, 512> algorithm;
	int* map;

	std::string line;
	// algorithm
	getline(input, line);
	if (line.length() != 512) {
			printf("Unexpected input\n");
			exit(-1);
	}
	for (int i = 0; i < 512; i++) {
		algorithm[i] = (line[i] == '#');
	}
	// empty line
	getline(input, line);
	// map
	int w = 0, h = 0;
	std::vector<std::string> lines;
	while (getline(input, line)) {
		lines.push_back(line);
		h++;
		if (w == 0) {
			w = line.length();
		} else if (w != line.length()) {
			printf("Unexpected input\n");
			exit(-1);
		}
	}

	// add border of 6 pixels everywhere
	w += 12;
	h += 12;
	map = new int[w * h];
	int xm, ym;
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			xm = x - 6;
			ym = y - 6;
			if ((xm < 0) || (xm >= w - 12)) {
				map[y * w + x] = 0;
			} else if ((ym < 0) || (ym >= h - 12)) {
				map[y * w + x] = 0;
			} else {
				map[y * w + x] = (lines[ym][xm] == '#');
			}
		}
	}

	int lit_pixel_count;

	for (int i = 0; i < 2; i++) {
		printf("Current map:\n");
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				printf("%c", (map[y * w + x] > 0) ? '#' : '.');
			}
			printf("\n");
		}
		printf("\n");

		int* new_map = new int[w * h];
		if ((i == 0) && (algorithm[0] == 1)) {
			for (int y = 0; y < h; y++) {
				for (int x = 0; x < w; x++) {
					new_map[y * w + x] = 1;
				}
			}
		}
		lit_pixel_count = 0;

		for (int y = 1; y < h - 1; y++) {
			for (int x = 1; x < w - 1; x++) {
				int value = 0;
				for (int y1 = y - 1; y1 <= y + 1; y1++) {
					for (int x1 = x - 1; x1 <= x + 1; x1++) {
						value = (value << 1) + map[y1 * w + x1];
					}
				}

				new_map[y * w + x] = algorithm[value];
				if (algorithm[value]) lit_pixel_count++;
			}
		}

		map = new_map;
	}

	printf("Final map:\n");
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			printf("%c", (map[y * w + x] > 0) ? '#' : '.');
		}
		printf("\n");
	}
	printf("\n");

	printf("lit pixel count: %i\n", lit_pixel_count);

	return 0;
}
