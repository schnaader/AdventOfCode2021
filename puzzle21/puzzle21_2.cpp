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

// #define USE_EXAMPLE_DATA // comment to use input target data, uncomment to use example target data

// LUT generator: histogram for the 7 distinct values of the dirac dice (1 + 1 + 1 = 3 .. 3 + 3 + 3 = 9)
/*void generate_lut() {
    int hist[10]{};
    for (int i = 1; i <= 3; i++) {
        for (int j = 1; j <= 3; j++) {
            for (int k = 1; k <= 3; k++) {
                hist[i + j + k]++;
            }
        }
    }

    for (int i = 0; i <= 9; i++) printf("%i: %i\n", i, hist[i]);
}*/

int distinct_values_count[10] = {0, 0, 0, 1, 3, 6, 7, 6, 3, 1}; // generated LUT

void count_universes(int which_player, int p1_space, int p2_space, int p1_score, int p2_score, int64_t& count_p1_wins, int64_t& count_p2_wins, int64_t multiplicator) {
    if (which_player == 1) {
        for (int value = 3; value <= 9; value++) {
            int p1_space_new = (p1_space + value) % 10;
            int p1_score_new = p1_score + (p1_space_new + 1);

            if (p1_score_new >= 21) {
                count_p1_wins += multiplicator * distinct_values_count[value];
            } else {
                count_universes(3 - which_player, p1_space_new, p2_space, p1_score_new, p2_score, count_p1_wins, count_p2_wins, multiplicator * distinct_values_count[value]);
            }
        }
    } else {
        for (int value = 3; value <= 9; value++) {
            int p2_space_new = (p2_space + value) % 10;
            int p2_score_new = p2_score + (p2_space_new + 1);

            if (p2_score_new >= 21) {
                count_p2_wins += multiplicator * distinct_values_count[value];
            } else {
                count_universes(3 - which_player, p1_space, p2_space_new, p1_score, p2_score_new, count_p1_wins, count_p2_wins, multiplicator * distinct_values_count[value]);
            }
        }
    }
}

int main() {
	int p1_start, p2_start;

	#ifdef USE_EXAMPLE_DATA
		// example 
		p1_start = 4;
		p2_start = 8;
	#else
		// input
		p1_start = 8;
		p2_start = 10;
	#endif

    // generate_lut();
    // return 0;

	int p1_score = 0, p2_score = 0;
	int p1_space = p1_start - 1, p2_space = p2_start - 1; // define spaces from 0..9 so modulo can be used

    int64_t universe_count_p1 = 0, universe_count_p2 = 0;
    count_universes(1, p1_space, p2_space, p1_score, p2_score, universe_count_p1, universe_count_p2, 1);

	printf("player 1 wins: %li\n", universe_count_p1);
	printf("player 2 wins: %li\n", universe_count_p2);

	return 0;
}
