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

int hex_to_byte(std::string s) {
	return std::stoi(s, nullptr, 16);
}

class Bitstream {
private:
	std::string hex_encoded;
	int idx;
	int bit_idx;
	int current_byte;

public:
	int current_bitpos;

	Bitstream(std::string s) {
		hex_encoded = s;
		bit_idx = 7;
		idx = 0;
		current_byte = hex_to_byte(hex_encoded.substr(idx * 2, 2));
		current_bitpos = 0;
	}

	int get_bit() {
		int result = (current_byte & (1 << bit_idx)) >> bit_idx;

		bit_idx--;
		current_bitpos++;
		if (bit_idx < 0) {
			bit_idx = 7;
			idx++;
			if (idx == hex_encoded.length()) {
				printf("Error: Read after end\n");
				exit(-1);
			}
			current_byte = hex_to_byte(hex_encoded.substr(idx * 2, 2));
		}

		return result;
	}

	uint64_t get_bits(int bitcount) {
		uint64_t result = 0;
		for (int i = 0; i < bitcount; i++) {
			result *= 2;
			result += get_bit();
		}

		return result;
	}
};

void sum_version(Bitstream& b, int& sum_so_far) {
	int version = b.get_bits(3);
	sum_so_far += version;
	printf("Version: %i\n", version);

	int type_id = b.get_bits(3);
	printf("Type ID: %i\n", type_id);
	if (type_id == 4) { // literal
		int last_group_if_0;
		int64_t literal = 0;
		do {
			last_group_if_0 = b.get_bit();
			literal = literal * 16 + b.get_bits(4);
		} while (last_group_if_0 == 1);
		printf("Literal %li\n", literal);
	} else { // operator
		int length_type_id = b.get_bit();
		if (length_type_id == 0) { // 15 bits - length of subpackets
			int subpkg_length = b.get_bits(15);
			printf("Subpkg length %i\n", subpkg_length);
			while (subpkg_length > 0) {
				int bitpos_before = b.current_bitpos;
				sum_version(b, sum_so_far);
				int bitpos_after = b.current_bitpos;
				subpkg_length -= bitpos_after - bitpos_before;
			}
		} else { // 11 bits - number of subpackets
			int subpkg_count = b.get_bits(11);
			printf("Subpkg count %i\n", subpkg_count);
			for (int i = 0; i < subpkg_count; i++) {
				sum_version(b, sum_so_far);
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

	std::vector<std::string> packets;

	std::string line;
	while (getline(input, line)) {
		packets.push_back(line);
	}

	int version_sum;
	for (auto packet : packets) {
		version_sum = 0;
		printf("Packet: %s\n", packet.c_str());
		Bitstream b(packet);
		sum_version(b, version_sum);
		printf("Version sum: %i\n\n", version_sum);
	}

	return 0;
}
