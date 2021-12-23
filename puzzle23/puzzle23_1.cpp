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
#include <climits>

//#define USE_EXAMPLE_DATA // comment to use input target data, uncomment to use example target data

                           // be patient, on my machine, example target data took 6 seconds, but
						   // the input target took much longer (some minutes)
enum State {START_IN_ROOM_BLOCKED, START_IN_ROOM_NOT_BLOCKED, OUT_IN_HALLWAY, FINISHED_IN_ROOM};

/* coordinate system and important coordinates:
#############
#...........# 2  ^
###.#.#.#.### 1  |
  #.#.#.#.#   0  Y      start positions:   X = 2, 4, 6, 8          ; Y = 0, 1
  #########             hallway positions: X = 0, 1, 3, 5, 7, 9, 10; Y = 2

           1
 01234567890

 X --> 
*/

// hallway X positions
std::array<int, 7> hallway_x = {0,1,3,5,7,9,10};
const int hallway_y = 2;

struct Pod {
	int x;
	int y;
	int target_x;
	int energy_per_move;
	State state;
	char c; // debug (used by show_map)
};

bool any_pod_on_pos(std::array<Pod, 8>& pods, int x, int y) {
	for (auto& pod: pods) {
		if ((pod.x == x) && (pod.y == y)) return true;
	}

	return false;
}

Pod pod_on_pos(std::array<Pod, 8>& pods, int x, int y) {
	Pod dummy_pod;
	dummy_pod.c = '.';

	for (auto& pod: pods) {
		if ((pod.x == x) && (pod.y == y)) return pod;
	}

	return dummy_pod;
}

void show_map(std::array<Pod, 8> pods) {
	printf("#############\n");
	printf("#%c%c.%c.%c.%c.%c%c#\n", pod_on_pos(pods, 0, 2).c, pod_on_pos(pods, 1, 2).c, pod_on_pos(pods, 3, 2).c, pod_on_pos(pods, 5, 2).c, pod_on_pos(pods, 7, 2).c, pod_on_pos(pods, 9, 2).c, pod_on_pos(pods, 10, 2).c);
	printf("###%c#%c#%c#%c###\n", pod_on_pos(pods, 2, 1).c, pod_on_pos(pods, 4, 1).c, pod_on_pos(pods, 6, 1).c, pod_on_pos(pods, 8, 1).c);
	printf("  #%c#%c#%c#%c#\n", pod_on_pos(pods, 2, 0).c, pod_on_pos(pods, 4, 0).c, pod_on_pos(pods, 6, 0).c, pod_on_pos(pods, 8, 0).c);
	printf("  #########\n");
}

int find_min_energy_sum(std::array<Pod, 8> pods, int energy_so_far, int min_energy_sum_so_far) {
	// more energy used than current minimum? break
	if (energy_so_far > min_energy_sum_so_far) {
		return INT_MAX;
	}

	// already finished?
	if (std::all_of(pods.begin(), pods.end(), [](Pod p) { return p.state == State::FINISHED_IN_ROOM; })) {
		//show_map(pods);
		//printf("Final energy: %i\n", energy_so_far);
		return energy_so_far;
	}

	//show_map(pods);
	//printf("%i\n", energy_so_far);

	int min_energy_possible_moves = min_energy_sum_so_far;
	std::array<Pod, 8> newPods = pods;
	int new_energy_so_far = energy_so_far;
	int pod_idx = 0;
	for (auto& pod: pods) {
		if (pod.state == State::START_IN_ROOM_NOT_BLOCKED) {
			// move out of room to one of the hallway positions
			for (auto& hx : hallway_x) {
				newPods = pods;
				new_energy_so_far = energy_so_far;

				// way on hallway free?
				int x_min = std::min(pod.x, hx);
				int x_max = std::max(pod.x, hx);
				bool way_is_free = true;
				for (int x = x_min; x <= x_max; x++) {
					if (any_pod_on_pos(pods, x, hallway_y)) {
						way_is_free = false;
						break;
					}
				}
				if (way_is_free) {
					newPods[pod_idx].state = State::OUT_IN_HALLWAY;
					new_energy_so_far += (abs(pod.x - hx) + abs(pod.y - hallway_y)) * pod.energy_per_move;
					newPods[pod_idx].x = hx;
					newPods[pod_idx].y = hallway_y;

					// since this pod moved to the hallway, the state of the pod below (if any)
					// progresses to NOT_BLOCKED
					if (pod.y == 1) {
						for (int i = 0; i < 8; i++) {
							if (true) {
								if ((pods[i].x == pod.x) && (pods[i].y == 0))
									if (pods[i].state == State::START_IN_ROOM_BLOCKED) {
										newPods[i].state = State::START_IN_ROOM_NOT_BLOCKED;
										break;
									}
							}
						}
					}

					int min_move_energy = find_min_energy_sum(newPods, new_energy_so_far, min_energy_sum_so_far);
					min_energy_possible_moves = std::min(min_energy_possible_moves, min_move_energy);
				}
			}
		} else if (pod.state == State::OUT_IN_HALLWAY) {
			newPods = pods;
			new_energy_so_far = energy_so_far;

			// way from hallway to target room free?
			int x_min = std::min(pod.x, pod.target_x);
			int x_max = std::max(pod.x, pod.target_x);
			bool way_is_free = true;
			for (int x = x_min; x <= x_max; x++) {
				if (x == pod.x) continue;

				if (any_pod_on_pos(pods, x, hallway_y)) {
					way_is_free = false;
					break;
				}
			}

			if (way_is_free) {
				// target room free? (empty or filled only with a finished pod on y = 0)
				bool target_room_free = true;
				bool target_room_one_pod_finished = false;

				if (any_pod_on_pos(pods, pod.target_x, 0)) {
					if (pod_on_pos(pods, pod.target_x, 0).state == State::FINISHED_IN_ROOM) {
						target_room_one_pod_finished = true;
						if (any_pod_on_pos(pods, pod.target_x, 1)) {
							target_room_free = false;
						}
					} else {
						target_room_free = false;
					}
				}

				if (target_room_free) {
					int newY = target_room_one_pod_finished ? 1 : 0;
					newPods[pod_idx].state = State::FINISHED_IN_ROOM;
					new_energy_so_far += (abs(pod.x - pod.target_x) + abs(pod.y - newY)) * pod.energy_per_move;
					newPods[pod_idx].x = pod.target_x;
					newPods[pod_idx].y = newY;

					int min_move_energy = find_min_energy_sum(newPods, new_energy_so_far, min_energy_sum_so_far);
					min_energy_possible_moves = std::min(min_energy_possible_moves, min_move_energy);
				}
			}
		}

		pod_idx++;
	}

	return min_energy_possible_moves;
}

int main() {
	std::array<Pod, 8> pods;
	#ifdef USE_EXAMPLE_DATA
		pods[0] = {.x = 2, .y = 0, .target_x = 2, .energy_per_move = 1};
		pods[1] = {.x = 8, .y = 0, .target_x = 2, .energy_per_move = 1};
		pods[2] = {.x = 2, .y = 1, .target_x = 4, .energy_per_move = 10};
		pods[3] = {.x = 6, .y = 1, .target_x = 4, .energy_per_move = 10};
		pods[4] = {.x = 4, .y = 1, .target_x = 6, .energy_per_move = 100};
		pods[5] = {.x = 6, .y = 0, .target_x = 6, .energy_per_move = 100};
		pods[6] = {.x = 4, .y = 0, .target_x = 8, .energy_per_move = 1000};
		pods[7] = {.x = 8, .y = 1, .target_x = 8, .energy_per_move = 1000};
	#else
		pods[0] = {.x = 4, .y = 1, .target_x = 2, .energy_per_move = 1};
		pods[1] = {.x = 8, .y = 0, .target_x = 2, .energy_per_move = 1};
		pods[2] = {.x = 2, .y = 1, .target_x = 4, .energy_per_move = 10};
		pods[3] = {.x = 6, .y = 1, .target_x = 4, .energy_per_move = 10};
		pods[4] = {.x = 2, .y = 0, .target_x = 6, .energy_per_move = 100};
		pods[5] = {.x = 8, .y = 1, .target_x = 6, .energy_per_move = 100};
		pods[6] = {.x = 4, .y = 0, .target_x = 8, .energy_per_move = 1000};
		pods[7] = {.x = 6, .y = 0, .target_x = 8, .energy_per_move = 1000};
	#endif
	for (int i = 0; i < 8; i++) {
		if (pods[i].y == 0) {
			if (pods[i].x == pods[i].target_x) {
				pods[i].state = State::FINISHED_IN_ROOM;
			} else {
				pods[i].state = State::START_IN_ROOM_BLOCKED;
			}
		} else {
			pods[i].state = State::START_IN_ROOM_NOT_BLOCKED;
		}

		switch (pods[i].energy_per_move) {
			case 1:
				pods[i].c = 'A';
				break;
			case 10:
				pods[i].c = 'B';
				break;
			case 100:
				pods[i].c = 'C';
				break;
			case 1000:
				pods[i].c = 'D';
				break;
		}
	}

	int energy_sum = 0;

	energy_sum = find_min_energy_sum(pods, 0, INT_MAX);

	printf("minimal energy sum: %i\n", energy_sum);

	return 0;
}
