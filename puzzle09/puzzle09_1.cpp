#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>

int main() {
	//std::ifstream input("example.txt");
	std::ifstream input("input.txt");

	if (!input.is_open()) {
		printf("Error opening file\n");
		return -1;
	}

	int64_t sum = 0;

	std::vector<std::string> lines;
	std::string line;
	while (getline(input, line)) {
		lines.push_back(line);
	}
	input.close();

	int w = lines[0].length();
	int h = lines.size();

	printf("Map dimensions: %i x %i\n", w, h);

	int* hmap = new int[w * h];
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			hmap[y * w + x] = lines[y][x] - '0';
			//printf("%i", hmap[y * w + x]);
		}
		//printf("\n");
	}

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			bool all_higher = true;
			int val = hmap[y * w + x];
			if (x > 0) {
				if (hmap[y * w + x - 1] <= val) all_higher = false;
			}
			if (x < (w - 1)) {
				if (hmap[y * w + x + 1] <= val) all_higher = false;
			}
			if (y > 0) {
				if (hmap[(y - 1) * w + x] <= val) all_higher = false;
			}
			if (y < (h - 1)) {
				if (hmap[(y + 1) * w + x] <= val) all_higher = false;
			}

			if (all_higher) {
				printf("%i\n", val);
				sum += (val + 1);
			}
		}
	}

	delete[] hmap;

	std::cout << "Sum: " << sum << "\n";

	return 0;
}
