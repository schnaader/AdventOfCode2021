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

//#define USE_EXAMPLE_DATA // comment to use input target data, uncomment to use example target data

int main() {
	int target_x_min, target_x_max;
	int target_y_min, target_y_max;

	#ifdef USE_EXAMPLE_DATA
		// example 
		target_x_min = 20;
		target_x_max = 30;
		target_y_min = -10;
		target_y_max = -5;
	#else
		// input
		target_x_min = 265;
		target_x_max = 287;
		target_y_min = -103;
		target_y_max = -58;
	#endif

	// For most inputs, part 1 can be solved using simple math.
	// X and Y values are independent
	// When the X velocity has gone done to 0, the X value is start_velX + (start_velX - 1) + ... + 1 = start_velX * (start_velX + 1) / 2
	// So sqrt(2 * target_x_min) is a good first guess
	//
	// For positive initial Y velocities, Y will first go negative, but rise and fall are symmetrical, so it will be exactly 0 later.
	// After that, it will have the absolute start velocity increased by 1 - which will hit the target, if the start velocity was abs(target_y_min) decreased by 1.
	
	int x_vel = (int)sqrt(target_x_min * 2); // first guess, might be too low
	while ((x_vel * (x_vel + 1) / 2) < target_x_min) {
		x_vel++;
	}
	if ((x_vel * (x_vel + 1) / 2) > target_x_max) {
		printf("x velocity didn't work out\n");
		return -1;
	}

	int y_vel = abs(target_y_min) - 1;

    // highest y position will be start_valY + (start_valY - 1) + ... + 1 = start_velY * (start_velY + 1) / 2
	int highest_y_pos = (y_vel * (y_vel + 1) / 2);
	printf("Highest Y position: %i\n", highest_y_pos);

	return 0;
}
