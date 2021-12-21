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

	int det_die = 0; // define die from 0..99 so modulo can be used
	int die_roll_count = 0;
	int p1_score = 0, p2_score = 0;
	int p1_space = p1_start - 1, p2_space = p2_start - 1; // define spaces from 0..9 so modulo can be used

	for (;;) {
		for (int i = 0; i < 3; i++) {
			p1_space = (p1_space + (det_die + 1)) % 10;
			det_die = (det_die + 1) % 100;
		}
		die_roll_count += 3;
		p1_score += (p1_space + 1);

		if (p1_score >= 1000) break;

		for (int i = 0; i < 3; i++) {
			p2_space = (p2_space + (det_die + 1)) % 10;
			det_die = (det_die + 1) % 100;
		}
		die_roll_count += 3;
		p2_score += (p2_space + 1);

		if (p2_score >= 1000) break;
	}

	int losing_score = std::min(p1_score, p2_score);

	printf("multiply result: %i * %i = %i\n", losing_score, die_roll_count, losing_score * die_roll_count);

	return 0;
}
