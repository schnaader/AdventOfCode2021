#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <array>
#include <stack>

struct Point {
	int x;
	int y;
};

enum FoldAxis {X, Y};

struct Fold {
	FoldAxis axis;
	int position;
};

int fold_w, fold_h;     // dimension of the used part of the dot map
size_t w, h;            // dot map dimensions
bool* dot_map;

void do_fold(Fold f) {
	if (f.axis == FoldAxis::X) {
		for (int x = f.position + 1; x < w; x++) {
			for (int y = 0; y < h; y++) {
				if (dot_map[y * w + x]) {
					dot_map[y * w + x] = false;
					dot_map[y * w + (2 * f.position - x)] = true;
				}
			}
		}

        // reduce used size
        fold_w = f.position;
	} else {
		for (int y = f.position + 1; y < h; y++) {
			for (int x = 0; x < w; x++) {
				if (dot_map[y * w + x]) {
					dot_map[y * w + x] = false;
					dot_map[(2 * f.position - y) * w + x] = true;
				}
			}
		}

        // reduce used size
        fold_h = f.position;
	}
}

int main() {
	//std::ifstream input("example.txt");
	std::ifstream input("input.txt");

	if (!input.is_open()) {
		printf("Error opening file\n");
		return -1;
	}

	int dot_count = 0;

	std::string line;
	std::vector<Point> coordinates;
	w = 0;
	h = 0;
	// parse part 1 (coordinates)
	while (getline(input, line)) {
		if (line == "") break; // empty line starts next section

		std::istringstream ls(line);
		std::string part1;
		std::string part2;
		getline(ls, part1, ',');
		getline(ls, part2, ',');

		// add two directed paths
		Point p;
		p.x = atoi(part1.c_str());
		w = std::max(w, (size_t)p.x);
		p.y = atoi(part2.c_str());
		h = std::max(h, (size_t)p.y);

		coordinates.push_back(p);
	}

	// parse part 2 (folds)
	std::vector<Fold> folds;
	while (getline(input, line)) {
		std::istringstream ls(line);
		std::string part1;
		std::string part2;
		getline(ls, part1, '=');
		getline(ls, part2, '=');

		Fold f;
		char axis = part1[part1.length() - 1];
		if (axis == 'x') {
			f.axis = FoldAxis::X;
		} else {
			f.axis = FoldAxis::Y;
		}
		f.position = atoi(part2.c_str());
		folds.push_back(f);
	}

	// increase max. coordinates so we'll allocate enough space
	w++;
	h++;
    fold_w = w;
    fold_h = h;
	// convert coordinates to dot map
	dot_map = new bool[w * h]();
	for (auto p: coordinates) {
		dot_map[p.y * w + p.x] = true;
	}

	for (int i = 0; i < w * h; i++) {
		if (dot_map[i]) dot_count++;
	}
	
	std::cout << "Dot count before folds: " << dot_count << "\n";

    for (auto fold: folds) {
	    do_fold(fold);
    }

    // show the map in console
    printf("Final map:\n");
	dot_count = 0;
	for (int y = 0; y < fold_h; y++) {
        for (int x = 0; x < fold_w; x++) {
            if (dot_map[y * w + x]) {
                printf("%c", '#');
            } else {
                printf("%c", '.');
            }
            //printf("%c", dot_map[y * w + x] ? '#' : '.');
        }
        printf("\n");
	}
    printf("\n");

	return 0;
}
