#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>

struct point {
	int x;
	int y;
};

struct segment {
	point p1;
	point p2;
};

int main() {
	//std::ifstream input("example.txt");
	std::ifstream input("input.txt");

	if (!input.is_open()) {
		printf("Error opening file\n");
		return -1;
	}

	std::vector<segment> segments;
	std::string line;
	while (getline(input, line)) {
		segment segm;

		// example line:
		// 28,837 -> 792,73
		// -> split by ' '
		// -> "28,837","->","792,73"
		// use first and third one and split them by comma
		
		std::istringstream sl(line);
		std::string p1, p2;
		getline(sl, p1, ' '); // use first one...
		getline(sl, p2, ' ');
		getline(sl, p2, ' '); // ... and third one
		std::istringstream p1l(p1);
		std::string x, y;
		getline(p1l, x, ',');
		getline(p1l, y, ',');
		segm.p1.x = atoi(x.c_str());
		segm.p1.y = atoi(y.c_str());

		std::istringstream p2l(p2);
		getline(p2l, x, ',');
		getline(p2l, y, ',');
		segm.p2.x = atoi(x.c_str());
		segm.p2.y = atoi(y.c_str());

		segments.push_back(segm);
	}
	
	const int w = 1000;
	const int MAPSIZE = w * w;
	std::array<int, MAPSIZE> map;
	for (int i = 0; i < MAPSIZE; i++) {
		map[i] = 0;
	}
	for (auto segm: segments) {
		int minx = std::min(segm.p1.x, segm.p2.x);
		int maxx = std::max(segm.p1.x, segm.p2.x);
		int miny = std::min(segm.p1.y, segm.p2.y);
		int maxy = std::max(segm.p1.y, segm.p2.y);

		if (segm.p1.x == segm.p2.x) {
			for (int y = miny; y <= maxy; y++) {
				map[y * w + segm.p1.x]++;
			}
		} else if (segm.p1.y == segm.p2.y) {
			for (int x = minx; x <= maxx; x++) {
				map[segm.p1.y * w + x]++;
			}			
		}
	}

	int avoid_point_count = 0;
	for (int i = 0; i < MAPSIZE; i++) {
		if (map[i] >= 2) avoid_point_count++;
	}
	printf("2 or larger count: %i\n", avoid_point_count);
}
