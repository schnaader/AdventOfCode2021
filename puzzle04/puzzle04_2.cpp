#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>

int main() {
	int sum_unmarked = 0;

	//std::ifstream input("example.txt");
	std::ifstream input("input.txt");

	if (!input.is_open()) {
		printf("Error opening file\n");
		return -1;
	}

	std::vector<int> bingo_numbers;
	std::string first_line;
	getline(input, first_line);
	std::istringstream fl(first_line);
	std::string number;
	while (getline(fl, number, ',')) {
		bingo_numbers.push_back(atoi(number.c_str()));
	}
	printf("Bingo number count: %li\n", bingo_numbers.size());

	std::vector<std::array<int, 25>> boards;
	for (;;) {
		std::string line;
		if (!getline(input, line)) break;

		std::array<int, 25> board;
		for (int y = 0; y < 5; y++) {
			getline(input, line);

			std::istringstream bl(line);
			for (int x = 0; x < 5; x++) {
				do {
					getline(bl, number, ' ');
				} while (number == "");
				board[y * 5 + x] = atoi(number.c_str());
			}
		}

		printf("Board %li:\n", boards.size());
		for (int y = 0; y < 5; y++) {
			for (int x = 0; x < 5; x++) {
				printf("%i ", board[y * 5 + x]);
			}
			printf("\n");
		}
		
		boards.push_back(board);
	}
	printf("Board count: %li\n", boards.size());
	
	std::vector<std::array<int, 25>> marks;
	std::vector<bool> board_has_bingo;
	for (int i = 0; i < boards.size(); i++) {
		std::array<int, 25> board;
		for (int j = 0; j < 25; j++) board[j] = 0;
		marks.push_back(board);
		board_has_bingo.push_back(false);
	}

	bool last_one = false;
	for (auto bingo_number : bingo_numbers) {
		printf("drawing %i\n", bingo_number);
		
		// mark number
		int board_number = 0;
		for (auto board : boards) {
			for (int j = 0; j < 25; j++) {
				if (board[j] == bingo_number) {
					marks[board_number][j] = 1;
				}
			}
			board_number++;
		}
		
		// check for bingo
		bool complete_line;
		board_number = 0;
		for (auto marked_board : marks) {
			complete_line = false;
			
			sum_unmarked = 0;
			//printf("\n\n");
			for (int y = 0; y < 5; y++) {
				bool complete_row = true, complete_col = true;
				for (int x = 0; x < 5; x++) {
					if (marked_board[y * 5 + x] == 0) complete_row = false;
					if (marked_board[x * 5 + y] == 0) complete_col = false;
					
					if (marked_board[y * 5 + x] == 0) {
						sum_unmarked += boards[board_number][y * 5 + x];
						//printf("sum(%i) -> %i ", boards[board_number][y * 5 + x], sum_unmarked);
					}
				}
				if (complete_col) {
					printf("Complete column on board %i\n", board_number);
					for (int y = 0; y < 5; y++) {
						for (int x = 0; x < 5; x++) {
							printf("%i ", marked_board[y * 5 + x]);
						}
						printf("\n");
					}
				}
				if (complete_row) {
					printf("Complete row on board %i\n", board_number);
					for (int y = 0; y < 5; y++) {
						for (int x = 0; x < 5; x++) {
							printf("%i ", marked_board[y * 5 + x]);
						}
						printf("\n");
					}
				}
				if (complete_col || complete_row) complete_line = true;
			}
			
			if (complete_line) {
				board_has_bingo[board_number] = true;
				/*for (auto b: board_has_bingo) {
					printf("%i ", (int)b);
				}
				printf("\n");*/
				if (std::all_of(board_has_bingo.begin(), board_has_bingo.end(), [](bool b){return b;})) {
					last_one = true;
					break;
				}
			}
			
			board_number++;
		}
		
		if (last_one) {			
			printf("sum unmarked: %i, last drawn bingo number: %i, product: %i\n", sum_unmarked, bingo_number, sum_unmarked * bingo_number);

			return 0;
		}
	}
}
