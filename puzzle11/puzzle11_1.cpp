#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <stack>

std::array<int, 100> field;
std::array<bool, 100> did_flash;
int64_t flash_count = 0;

void flash() {
	for (int y = 0; y < 10; y++) {
		for (int x = 0; x < 10; x++) {
			did_flash[y * 10 + x] = false;
		}
	}
	for (;;) {
		bool none_flashed = true;
		for (int y = 0; y < 10; y++) {
			for (int x = 0; x < 10; x++) {
				if ((field[y * 10 + x] > 9) && (!did_flash[y * 10 + x])) {
					//printf("Field %i, %i is flashed\n", x, y);
					flash_count++;
					none_flashed = false;
					did_flash[y * 10 + x] = true;
					for (int yd = 1; yd <= 1; yd++) {
						for (int xd = 1; xd <= 1; xd++) {
							if ((x - xd) >= 0) {
								field[y * 10 + x - xd]++;
								if ((y - yd) >= 0) {
									field[(y - yd) * 10 + x - xd]++;
								}
							}
							if ((x + xd) <= 9) {
								field[y * 10 + x + xd]++;
								if ((y + yd) <= 9) {
									field[(y + yd) * 10 + x + xd]++;
								}
							}
							if ((y - yd) >= 0) {
								field[(y - yd) * 10 + x]++;
								if ((x + xd) <= 9) {
									field[(y - yd) * 10 + x + xd]++;
								}
							}
							if ((y + yd) <= 9) {
								field[(y + yd) * 10 + x]++;
								if ((x - xd) >= 0) {
									field[(y + yd) * 10 + x - xd]++;
								}
							}
						}
					}
				}
			}
		}
		if (none_flashed) break;
	}
	for (int y = 0; y < 10; y++) {
		for (int x = 0; x < 10; x++) {
			if (did_flash[y * 10 + x]) {
				field[y * 10 + x] = 0;
			}
		}
	}
}

int main() {
	//std::ifstream input("example.txt");
	std::ifstream input("input.txt");

	if (!input.is_open()) {
		printf("Error opening file\n");
		return -1;
	}

	std::string line;
	std::array<std::string, 10> strings;
	int i = 0;
	while (getline(input, line)) {
		strings[i] = line;
		i++;
	}

	for (int y = 0; y < 10; y++) {
		for (int x = 0; x < 10; x++) {
			const char* l = strings[y].c_str();
			field[y * 10 + x] = (int)(l[x] - '0');
		}
	}
	
	for (int i = 1; i < 101; i++) {
		for (int y = 0; y < 10; y++) {
			for (int x = 0; x < 10; x++) {
				field[y * 10 + x]++;
			}
		}
		
		flash();
		
		printf("After step %i:\n", i);
		for (int y = 0; y < 10; y++) {
			for (int x = 0; x < 10; x++) {
				printf("%i", field[y * 10 + x]);
			}
			printf("\n");
		}		
		
		std::cout << "Flash count after " << i << " steps: " << flash_count << "\n";
	}

	std::cout << "Flash count after 100 steps: " << flash_count << "\n";

	return 0;
}
