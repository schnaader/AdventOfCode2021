#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <stack>

int main() {
	//std::ifstream input("example.txt");
	std::ifstream input("input.txt");

	if (!input.is_open()) {
		printf("Error opening file\n");
		return -1;
	}

	int64_t score[256];
	score[')'] = 3;
	score[']'] = 57;
	score['}'] = 1197;
	score['>'] = 25137;

	int64_t sum = 0;

	std::string line;
	while (getline(input, line)) {
		std::stack<char> st;
		bool valid = true;
		char first_illegal;
		for (auto c : line) {
			switch (c) {
				case '(':
					st.push(')');
					break;
				case '[':
					st.push(']');
					break;
				case '{':
					st.push('}');
					break;
				case '<':
					st.push('>');
					break;
				case ')':
				case ']':
				case '}':
				case '>':
					if (st.top() != c) {
						first_illegal = c;
						valid = false;
						break;
					}
					st.pop();
					break;
			}
			
			if (!valid) {
				sum += score[first_illegal];
				printf("Invalid %c, new score: %li\n", first_illegal, sum);
				break;
			}
		}
	}

	std::cout << "Sum: " << sum << "\n";

	return 0;
}
