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
	score[')'] = 1;
	score[']'] = 2;
	score['}'] = 3;
	score['>'] = 4;

	std::vector<int64_t> line_scores;
	
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
				break;
			}
		}
		
		if (valid) { // valid
			if (st.size() > 0) { // incomplete
				printf("Valid, but incomplete line: %s\n", line.c_str());
				int64_t line_score = 0;
				while (st.size() > 0) {
					line_score = line_score * 5 + score[st.top()];
					st.pop();
				}
				printf("Line score: %li\n", line_score);
				line_scores.push_back(line_score);
			}
		}
	}
	std::sort(line_scores.begin(), line_scores.end());

	std::cout << "Top score: " << line_scores[line_scores.size() - 1] << "\n";

	std::cout << "Middle score: " << line_scores[line_scores.size() / 2] << "\n";

	return 0;
}
