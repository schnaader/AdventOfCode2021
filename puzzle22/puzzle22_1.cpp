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

struct Cuboid {
	bool is_on;
	int minX, maxX;
	int minY, maxY;
	int minZ, maxZ;
};

int main() {
	//std::ifstream input("example1.txt");
	//std::ifstream input("example2.txt");
	std::ifstream input("input.txt");

	if (!input.is_open()) {
		printf("Error opening file\n");
		return -1;
	}

	std::vector<Cuboid> cuboids;
	std::string line;
	while (getline(input, line)) {
		Cuboid c;

		std::istringstream ls(line);
		std::string part;
		getline(ls, part, ' ');
		if (part == "on") {
			c.is_on = true;
		} else if (part == "off") {
			c.is_on = false;
		} else {
			printf("Unexpected input\n");
			return -1;
		}
		getline(ls, part, '=');
		getline(ls, part, '.');
		c.minX = atoi(part.c_str());
		getline(ls, part, '.');
		getline(ls, part, ',');
		c.maxX = atoi(part.c_str());

		getline(ls, part, '=');
		getline(ls, part, '.');
		c.minY = atoi(part.c_str());
		getline(ls, part, '.');
		getline(ls, part, ',');
		c.maxY = atoi(part.c_str());

		getline(ls, part, '=');
		getline(ls, part, '.');
		c.minZ = atoi(part.c_str());
		getline(ls, part, '.');
		getline(ls, part, ',');
		c.maxZ = atoi(part.c_str());
		
		cuboids.push_back(c);
	}

	bool* grid = new bool[101*101*101]{false}; // true = on

	for (auto c : cuboids) {
		if ((c.minX >= -50) && (c.maxX <= 50)) {
			if ((c.minY >= -50) && (c.maxY <= 50)) {
				if ((c.minZ >= -50) && (c.maxZ <= 50)) {
					for (int z = c.minZ; z <= c.maxZ; z++) {
						for (int y = c.minY; y <= c.maxY; y++) {
							for (int x = c.minX; x <= c.maxX; x++) {
								grid[(z + 50) * 101 * 101 + (y + 50) * 101 + (x + 50)] = c.is_on;
							}
						}						
					}
				}
			}
		}
	}

	int cubes_on_count = 0;
	for (int i = 0; i < 101*101*101; i++) {
		if (grid[i]) cubes_on_count++;
	}

	printf("cubes on count: %i\n", cubes_on_count);

	return 0;
}
