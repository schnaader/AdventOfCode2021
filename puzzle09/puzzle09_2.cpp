#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>

int* hmap;

void flood_fill(int x, int y, int w, int h, int& size) {
	if (hmap[y * w + x] == 9) return;

	hmap[y * w + x] = 9;
	size++;

	if (x > 0) {
		flood_fill(x - 1, y, w, h, size);
	}
	if (x < w - 1) {
		flood_fill(x + 1, y, w, h, size);
	}
	if (y > 0) {
		flood_fill(x, y - 1, w, h, size);
	}
	if (y < h - 1) {
		flood_fill(x, y + 1, w, h, size);
	}
}

int main() {
	//std::ifstream input("example.txt");
	std::ifstream input("input.txt");

	if (!input.is_open()) {
		printf("Error opening file\n");
		return -1;
	}

	int64_t product = 1;

	std::vector<std::string> lines;
	std::string line;
	while (getline(input, line)) {
		lines.push_back(line);
	}
	input.close();

	int w = lines[0].length();
	int h = lines.size();

	printf("Map dimensions: %i x %i\n", w, h);

	hmap = new int[w * h];
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			hmap[y * w + x] = lines[y][x] - '0';
			//printf("%i", hmap[y * w + x]);
		}
		//printf("\n");
	}

	std::vector<int> sizes;
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
				int size = 0;
				flood_fill(x, y, w, h, size);
				printf("size: %i\n", size);
				sizes.push_back(size);
			}
		}
	}

	delete[] hmap;

	std::sort(sizes.begin(), sizes.end());
	std::reverse(sizes.begin(), sizes.end());

	printf("\nThree largest sizes:\n");
	for (int i = 0; i < 3; i++) {
		printf("%i ", sizes[i]);
		product *= sizes[i];
	}
	printf("\n");

	std::cout << "Product: " << product << "\n";

	return 0;
}
