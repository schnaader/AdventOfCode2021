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
#include <numeric>

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
			if (idx < (hex_encoded.length() / 2)) {
			    current_byte = hex_to_byte(hex_encoded.substr(idx * 2, 2));
            } else {
                current_byte = -1;
            }
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

int64_t get_value(Bitstream& b) {
    int64_t result = 0;

	int version = b.get_bits(3);
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

        result = literal;
	} else { // operator
        std::vector<int64_t> subpkg_values;

		int length_type_id = b.get_bit();
		if (length_type_id == 0) { // 15 bits - length of subpackets
			int subpkg_length = b.get_bits(15);
			printf("Subpkg length %i\n", subpkg_length);
			while (subpkg_length > 0) {
				int bitpos_before = b.current_bitpos;
				subpkg_values.push_back(get_value(b));
				int bitpos_after = b.current_bitpos;
				subpkg_length -= bitpos_after - bitpos_before;
			}
		} else { // 11 bits - number of subpackets
			int subpkg_count = b.get_bits(11);
			printf("Subpkg count %i\n", subpkg_count);
			for (int i = 0; i < subpkg_count; i++) {
				subpkg_values.push_back(get_value(b));
			}
		}

        int64_t const & (*min) (int64_t const &, int64_t const &) = std::min<int64_t>;
        int64_t const & (*max) (int64_t const &, int64_t const &) = std::max<int64_t>;
        result = 0;

        switch (type_id) {
            case 0: // sum
                printf("Sum of %li values\n", subpkg_values.size());
                result = std::accumulate(subpkg_values.begin(), subpkg_values.end(), (int64_t)0);
                break;
            case 1: // product
                printf("Product of %li values\n", subpkg_values.size());
                result = std::accumulate(subpkg_values.begin(), subpkg_values.end(), (int64_t)1, std::multiplies<int64_t>());
                break;
            case 2: // minimum
                printf("Minimum of %li values\n", subpkg_values.size());
                result = std::accumulate(subpkg_values.begin() + 1, subpkg_values.end(),
                                        subpkg_values.front(), min);
                break;
            case 3: // maximum
                printf("Maximum of %li values\n", subpkg_values.size());
                result = std::accumulate(subpkg_values.begin() + 1, subpkg_values.end(),
                                        subpkg_values.front(), max);
                break;
            case 5: // greater than
                if (subpkg_values.size() != 2) {
                    printf("Greater than, not 2 values!\n");
                    exit(-1);
                }
                if (subpkg_values[0] > subpkg_values[1]) result = 1;
                break;
            case 6: // less than
                if (subpkg_values.size() != 2) {
                    printf("Less than, not 2 values!\n");
                    exit(-1);
                }
                if (subpkg_values[0] < subpkg_values[1]) result = 1;
                break;
            case 7: // equal to
                if (subpkg_values.size() != 2) {
                    printf("Equal to, not 2 values!\n");
                    exit(-1);
                }
                if (subpkg_values[0] == subpkg_values[1]) result = 1;
                break;
        }

        for (auto v : subpkg_values) {
            printf("%li ", v);
        }
        printf(" => %li\n", result);
	}

    return result;
}

int main() {
    //std::ifstream input("example2.txt");
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
		int64_t value = get_value(b);
		printf("Value: %li\n\n", value);
	}

	return 0;
}
