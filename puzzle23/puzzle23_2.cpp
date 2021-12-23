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

                           // be patient, both example target data and input target data take about 20 minutes

enum State {START_IN_ROOM_BLOCKED, START_IN_ROOM_NOT_BLOCKED, OUT_IN_HALLWAY, FINISHED_IN_ROOM};

/* coordinate system and important coordinates:
#############
#...........# 4  
###.#.#.#.### 3  
  #.#.#.#.#   2  ^ 
  #.#.#.#.#   1  |
  #.#.#.#.#   0  Y      start positions:   X = 2, 4, 6, 8          ; Y = 0, 1, 2, 3
  #########             hallway positions: X = 0, 1, 3, 5, 7, 9, 10; Y = 4

           1
 01234567890

 X --> 
*/

// hallway X positions
std::array<int, 7> hallway_x = {0,1,3,5,7,9,10};
const int hallway_y = 4;

struct Pod {
	int x;
	int y;
	int target_x;
	int energy_per_move;
	State state;
};

bool any_pod_on_pos(std::array<Pod, 16>& pods, int x, int y) {
	for (auto& pod: pods) {
		if ((pod.x == x) && (pod.y == y)) return true;
	}

	return false;
}

Pod pod_on_pos(std::array<Pod, 16>& pods, int x, int y) {
	Pod dummy_pod;

	for (auto& pod: pods) {
		if ((pod.x == x) && (pod.y == y)) return pod;
	}

	return dummy_pod;
}

int find_min_energy_sum(std::array<Pod, 16> pods, int energy_so_far, int min_energy_sum_so_far) {
	// more energy used than current minimum? break
	if (energy_so_far > min_energy_sum_so_far) {
		return INT_MAX;
	}

	// already finished?
	if (std::all_of(pods.begin(), pods.end(), [](Pod p) { return p.state == State::FINISHED_IN_ROOM; })) {
		return energy_so_far;
	}

	int min_energy_possible_moves = min_energy_sum_so_far;
	std::array<Pod, 16> newPods = pods;
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
					if (pod.y > 0) {
						for (int i = 0; i < 16; i++) {
							if (true) {
								if ((pods[i].x == pod.x) && (pods[i].y == (pod.y - 1)))
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
				// target room free? (empty or filled only with finished pods on y = 2..0)
				bool target_room_free = true;
				int target_room_free_y = 0;

                if (any_pod_on_pos(pods, pod.target_x, 3)) {
                    target_room_free = false;
                } else {
                    for (int y = 2; y >= 0; y--) {
                        if (any_pod_on_pos(pods, pod.target_x, y)) {
                            if (pod_on_pos(pods, pod.target_x, y).state == State::FINISHED_IN_ROOM) {
                                target_room_free_y = y + 1;
                                break;
                            } else {
                                target_room_free = false;
                            }
                        }

                        if (!target_room_free) break;
                    }
                }


				if (target_room_free) {
					int newY = target_room_free_y;
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
	std::array<Pod, 16> pods;
	#ifdef USE_EXAMPLE_DATA
		pods[0] = {.x = 2, .y = 0, .target_x = 2, .energy_per_move = 1};
		pods[1] = {.x = 8, .y = 0, .target_x = 2, .energy_per_move = 1};
		pods[2] = {.x = 6, .y = 1, .target_x = 2, .energy_per_move = 1};
		pods[3] = {.x = 8, .y = 2, .target_x = 2, .energy_per_move = 1};
		pods[4] = {.x = 2, .y = 3, .target_x = 4, .energy_per_move = 10};
		pods[5] = {.x = 6, .y = 3, .target_x = 4, .energy_per_move = 10};
		pods[6] = {.x = 4, .y = 1, .target_x = 4, .energy_per_move = 10};
		pods[7] = {.x = 6, .y = 2, .target_x = 4, .energy_per_move = 10};
		pods[8] = {.x = 4, .y = 3, .target_x = 6, .energy_per_move = 100};
		pods[9] = {.x = 6, .y = 0, .target_x = 6, .energy_per_move = 100};
		pods[10] = {.x = 4, .y = 2, .target_x = 6, .energy_per_move = 100};
		pods[11] = {.x = 8, .y = 1, .target_x = 6, .energy_per_move = 100};
		pods[12] = {.x = 4, .y = 0, .target_x = 8, .energy_per_move = 1000};
		pods[13] = {.x = 8, .y = 3, .target_x = 8, .energy_per_move = 1000};
		pods[14] = {.x = 2, .y = 1, .target_x = 8, .energy_per_move = 1000};
		pods[15] = {.x = 2, .y = 2, .target_x = 8, .energy_per_move = 1000};
	#else
		pods[0] = {.x = 4, .y = 3, .target_x = 2, .energy_per_move = 1};
		pods[1] = {.x = 8, .y = 0, .target_x = 2, .energy_per_move = 1};
		pods[2] = {.x = 6, .y = 1, .target_x = 2, .energy_per_move = 1};
		pods[3] = {.x = 8, .y = 2, .target_x = 2, .energy_per_move = 1};
		pods[4] = {.x = 2, .y = 3, .target_x = 4, .energy_per_move = 10};
		pods[5] = {.x = 6, .y = 3, .target_x = 4, .energy_per_move = 10};
		pods[6] = {.x = 4, .y = 1, .target_x = 4, .energy_per_move = 10};
		pods[7] = {.x = 6, .y = 2, .target_x = 4, .energy_per_move = 10};
		pods[8] = {.x = 2, .y = 0, .target_x = 6, .energy_per_move = 100};
		pods[9] = {.x = 8, .y = 3, .target_x = 6, .energy_per_move = 100};
		pods[10] = {.x = 4, .y = 2, .target_x = 6, .energy_per_move = 100};
		pods[11] = {.x = 8, .y = 1, .target_x = 6, .energy_per_move = 100};
		pods[12] = {.x = 4, .y = 0, .target_x = 8, .energy_per_move = 1000};
		pods[13] = {.x = 6, .y = 0, .target_x = 8, .energy_per_move = 1000};
		pods[14] = {.x = 2, .y = 1, .target_x = 8, .energy_per_move = 1000};
		pods[15] = {.x = 2, .y = 2, .target_x = 8, .energy_per_move = 1000};
	#endif
	for (int i = 0; i < 16; i++) {
		if (pods[i].y == 0) {
			if (pods[i].x == pods[i].target_x) {
				pods[i].state = State::FINISHED_IN_ROOM;
			} else {
				pods[i].state = State::START_IN_ROOM_BLOCKED;
			}
		} else {
            if (pods[i].y < 3) {
			    pods[i].state = State::START_IN_ROOM_BLOCKED;
            } else {
                pods[i].state = State::START_IN_ROOM_NOT_BLOCKED;
            }
		}
	}

	int energy_sum = 0;

	energy_sum = find_min_energy_sum(pods, 0, INT_MAX);

	printf("minimal energy sum: %i\n", energy_sum);

	return 0;
}
