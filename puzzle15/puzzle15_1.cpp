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

std::array<int, 100*100> map;
int w, h;

double distance(int x1, int y1, int x2, int y2) {
	int dx = x2 - x1;
	int dy = y2 - y1;
	return sqrt(dx * dx + dy*dy);
}

struct Node {
	int id;
	int x;
	int y;
	bool is_open;
	double risk_so_far;
	double estimated_risk;
	double risk_sum;
};

Node create_node(int id, int x, int y, double risk_so_far) {
	Node result;
	
	result.id = 0;
	result.is_open = true;
	result.x = x;
	result.y = y;
	result.risk_so_far = risk_so_far;
	result.estimated_risk = distance(x, y, w - 1, h - 1);
	result.risk_sum = risk_so_far + result.estimated_risk;

	return result;
}

int main() {
	//std::ifstream input("example.txt");
	std::ifstream input("input.txt");

	if (!input.is_open()) {
		printf("Error opening file\n");
		return -1;
	}

	w = 0;
	h = 0;
	std::string line;
	while (getline(input, line)) {
		if (w == 0) {
 			w = line.length();
		} else if (w != line.length()) {
			printf("Error: Input is not consistent\n");
			return -1;
		}
		for (int x = 0; x < w; x++) {
			map[h * w + x] = line[x] - '0';
		}
		h++;
	}

	std::vector<Node> nodes;
	Node start = create_node(0, 0, 0, 0);
	nodes.push_back(start);

	auto begin = std::chrono::steady_clock::now();

	for (;;) {
		// find (open) node with smallest risk sum
		auto sorted_nodes = nodes;
		std::erase_if(sorted_nodes, [](Node n) { return !n.is_open; });
		std::sort(sorted_nodes.begin(), sorted_nodes.end(), [](Node a, Node b) {
			return a.risk_sum < b.risk_sum;
		});
		auto smallest_risk_node = sorted_nodes[0];
		//printf("Smallest risk at (%i, %i)\n", smallest_risk_node.x, smallest_risk_node.y);

		// close node ...
		for (auto& n : nodes) {
			if (n.id == smallest_risk_node.id) {
				n.is_open = false;
			}
		}

		// ... and expand
		std::vector<Node> expanded_nodes;
		int x = smallest_risk_node.x;
		int y = smallest_risk_node.y;
		int r = smallest_risk_node.risk_so_far;
		if (x > 0) {
			expanded_nodes.push_back(create_node(0, x - 1, y, r + map[y * w + x - 1]));
		}
		if (x < w - 1) {
			expanded_nodes.push_back(create_node(0, x + 1, y, r + map[y * w + x + 1]));
		}
		if (y > 0) {
			expanded_nodes.push_back(create_node(0, x, y - 1, r + map[(y - 1) * w + x]));
		}
		if (y < h - 1) {
			expanded_nodes.push_back(create_node(0, x, y + 1, r + map[(y + 1) * w + x]));
		}

		// only add expanded nodes if the position is new - else check if old node has to be replaced
		// also check if any of the expanded nodes reached the goal
		for (auto& e : expanded_nodes) {
			if ((e.x == w - 1) && (e.y == h - 1)) {
				std::cout << "Total risk: " << e.risk_sum << "\n";

				auto end = std::chrono::steady_clock::now();
				std::cout << "Elapsed time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms\n";

				return 0;
			}

			bool position_is_new = true;
			for (auto& n : nodes) {
				if ((n.x == e.x) && (n.y == e.y)) {
					position_is_new = false;
					if (e.risk_sum < n.risk_sum) {
						n.risk_so_far = e.risk_so_far;
						n.estimated_risk = e.estimated_risk;
						n.risk_sum = e.risk_sum;
						n.is_open = true;
					}
					break;
				}
			}

			if (position_is_new) {
				e.id = nodes.size();
				nodes.push_back(e);
			}
		}

		//printf("Node count: %li\n", nodes.size());
	}

	return 0;
}
