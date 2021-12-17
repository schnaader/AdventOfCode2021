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

    int in_target_count = 0;

    int x_vel, y_vel;
    for (y_vel = target_y_min; y_vel <= abs(target_y_min); y_vel++) {
        for (x_vel = 1; x_vel <= target_x_max; x_vel++) {
            int curr_x_vel = x_vel, curr_y_vel = y_vel;
            int x_pos = 0, y_pos = 0;

            while (y_pos >= target_y_min) {
                x_pos += curr_x_vel;
                y_pos += curr_y_vel;

                if ((x_pos >= target_x_min) && (x_pos <= target_x_max)) {
                    if ((y_pos >= target_y_min) && (y_pos <= target_y_max)) {
                        printf("%i,%i; ", x_vel, y_vel);
                        in_target_count++;
                        break;
                    }
                }

                if (curr_x_vel < 0) {
                    curr_x_vel++;
                } else if (curr_x_vel > 0) {
                    curr_x_vel--;
                }
                curr_y_vel--;
            }
        }
    }

    printf("\n\n In target count: %i\n", in_target_count);

	return 0;
}

/*
23,-10  25,-9   27,-5   29,-6   22,-6   21,-7   9,0     27,-7   24,-5
25,-7   26,-6   25,-5   6,8     11,-2   20,-5   29,-10  6,3     28,-7
8,0     30,-6   29,-8   6,7     6,4     6,1     14,-4   21,-6
26,-10  7,-1    7,7     8,-1    21,-9   6,2     20,-7   30,-10  14,-3
20,-8   13,-2   7,3     28,-8   29,-9   15,-3   22,-5   26,-8   25,-8
25,-6   15,-4   9,-2    15,-2   12,-2   28,-9   12,-3   24,-6   23,-7
25,-10  7,8     11,-3   26,-7   7,1     23,-9   6,0     22,-10  27,-6
8,1     22,-8   13,-4   7,6     28,-6   11,-4   12,-4   26,-9   7,4
24,-10  23,-8   30,-8   7,0     9,-1    10,-1   26,-5   22,-9   6,5
7,5     23,-6   28,-10  10,-2   11,-1   20,-9   14,-2   29,-7   13,-3
23,-5   24,-8   27,-9   30,-7   28,-5   21,-10  7,9     6,6     21,-5
27,-10  7,2     30,-9   21,-8   22,-7   24,-9   20,-6   6,9     29,-5
8,-2    27,-8   30,-5   24,-7

*/