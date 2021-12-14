#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <stack>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <algorithm>

struct extension_step {
    std::string polymer;
    int steps;

    // implement equality operator to be able to use as std::unordered_set key
    bool operator==(const extension_step &other) const
    { return (polymer == other.polymer
                && steps == other.steps);
    }
};

// implement hash to be able to use as std::unordered_set key
namespace std {

  template <>
  struct hash<extension_step>
  {
    std::size_t operator()(const extension_step& k) const
    {
      using std::size_t;
      using std::hash;
      using std::string;

      // Compute individual hash values for first,
      // second and third and combine them using XOR
      // and bit shifting:

      return hash<string>()(k.polymer)
               ^ (hash<int>()(k.steps) << 1);
    }
  };
}

std::map<std::string, char> rules;
std::map<char, int64_t> character_counts;
std::unordered_map<extension_step, std::map<char, int64_t>> cached_extension_results;

std::map<char, int64_t> counts(std::string polymer, int steps) {
    std::map<char, int64_t> result;

    if (steps == 0) {
        result[polymer[0]] = 1;
        if (polymer[1] == polymer[0]) {
            result[polymer[0]]++;
        } else {
            result[polymer[1]] = 1;
        }
        return result;
    }

    extension_step e;
    e.polymer = polymer;
    e.steps = steps;

    if (cached_extension_results.contains(e)) {
        result = cached_extension_results[e];
    } else {
        // expand polymer to two strings (e.g. NN => NCN => NC, CN)
        std::string p1 = std::string("") + polymer[0] + rules[polymer];
        std::string p2 = std::string("") + rules[polymer] + polymer[1];
        auto result1 = counts(p1, steps - 1);
        for (auto c : result1) {
            if (result.contains(c.first)) {
                result[c.first] += c.second;
            } else {
                result[c.first] = c.second;
            }
        }

        auto result2 = counts(p2, steps - 1);
        for (auto c : result2) {
            if (result.contains(c.first)) {
                result[c.first] += c.second;
            } else {
                result[c.first] = c.second;
            }
        }

        // the two results share the "rules" character (e.g. NC and CN share the C), so decrement that one
        result[rules[polymer]]--;

        cached_extension_results[e] = result;
    }

    return result;
}

int main() {
	//std::ifstream input("example.txt");
	std::ifstream input("input.txt");

	if (!input.is_open()) {
		printf("Error opening file\n");
		return -1;
	}

	std::string current_polymer;

	std::string line;
	// polymer template
	getline(input, line);
	current_polymer = line;
	// empty line
	getline(input, line);
	// pair insertion rules
	while (getline(input, line)) {
		std::string s = std::string("") + line[0] + line[1];
		rules.insert(std::pair<std::string, char>(s, line[line.length() - 1]));

	}

	// debug: print rules
	/*printf("rules:\n");
	for (auto r : rules) {
		printf("%s -> %c\n", r.first.c_str(), r.second);
	}
	printf("\n");*/

	printf("Template: %s\n", current_polymer.c_str());
	const int num_steps = 40;
    char c1 = current_polymer[0];
    char c2 = current_polymer[1];
    for (int i = 0; i < current_polymer.length() - 1; i++) {
	    std::string both = std::string("") + c1 + c2;

        //std::cout << "Expanding " << both << "...\n";
        auto result = counts(both, num_steps);
        for (auto c : result) {
            //std::cout << "Count of " << c.first << ": " << c.second << "\n";
            if (character_counts.contains(c.first)) {
                character_counts[c.first] += c.second;
            } else {
                character_counts[c.first] = c.second;
            }
        }

        if (i < current_polymer.length() - 2) {
            character_counts[c2]--;
            c1 = c2;
            c2 = current_polymer[i + 2];
        }
    }
    std::cout << "\n";

	int64_t most_common_count = character_counts.begin()->second;
    int64_t least_common_count = character_counts.begin()->second;
    char most_common_char = character_counts.begin()->first;
    char least_common_char = character_counts.begin()->first;

    std::cout << "After " << num_steps << " steps:\n";
	for (auto c: character_counts) {
        std::cout << "Count of " << c.first << ": " << c.second << "\n";
        
		least_common_count = std::min(least_common_count, c.second);
        if (c.second == least_common_count) least_common_char = c.first;
		most_common_count = std::max(most_common_count, c.second);
        if (c.second == most_common_count) most_common_char = c.first;
	}
    std::cout << "\n";

	std::cout << "Most common after " << num_steps << " steps: " << most_common_char << ", count:" << most_common_count << "\n";
	std::cout << "Least common after " << num_steps << " steps: " << least_common_char << ", count:" << least_common_count << "\n";
	std::cout << "Result: " << most_common_count - least_common_count << "\n";

	return 0;
}