#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <array>
#include <stack>

struct path {
	std::string from;
	std::string to;
};

std::vector<path> connections;

bool is_big_cave(std::string s) {
	return std::all_of(s.begin(), s.end(), [](char c){ return std::isupper(c); });
}

void count_paths(std::string start_node, int& path_count, std::unordered_set<std::string> used_caves, std::string way_so_far, std::string which_one_visited_twice) {
	if (start_node == "end") {
		printf("%s\n", way_so_far.c_str());
		path_count++;
		return;
	}
	
	for (auto connection: connections) {
		if (connection.from != start_node) continue;
		if (connection.to == "start") continue; // only visit start node once

		bool src_is_big = is_big_cave(connection.from);
		bool dst_is_big = is_big_cave(connection.to);
		
		bool go_on = true;
		std::string new_which_one_visited_twice = which_one_visited_twice;
		if (!dst_is_big) { // small caves ...
			if (used_caves.contains(connection.to)) { // can only be visited once ...
				if (which_one_visited_twice == "") { // except one that can be visited twice
					new_which_one_visited_twice = connection.to;
				} else {
					go_on = false;
				}
			}
		}
		
		if (go_on) {
			std::unordered_set<std::string> new_used_caves = used_caves;
			if (!src_is_big) {
				new_used_caves.insert(connection.from);
			}
			if (!dst_is_big) {
				new_used_caves.insert(connection.to);
			}
			count_paths(connection.to, path_count, new_used_caves, way_so_far + "," + connection.to, new_which_one_visited_twice);
		}
	}
}

int main() {
	//std::ifstream input("example_10.txt"); // expected now: 36
	//std::ifstream input("example_19.txt"); // expected now: 103
	//std::ifstream input("example_226.txt"); // expected now: 3509
	std::ifstream input("input.txt");

	if (!input.is_open()) {
		printf("Error opening file\n");
		return -1;
	}

	std::string line;
	while (getline(input, line)) {
		std::istringstream ls(line);
		std::string part1;
		std::string part2;
		getline(ls, part1, '-');
		getline(ls, part2, '-');

		// add two directed paths
		path p1, p2;
		p1.from = part1;
		p1.to = part2;
		p2.from = part2;
		p2.to = part1;
				
		connections.push_back(p1);
		connections.push_back(p2);
	}

	int path_count = 0;
	std::unordered_set<std::string> used_caves;

	count_paths("start", path_count, used_caves, "start", "");

	std::cout << "Path count: " << path_count << "\n";

	return 0;
}
