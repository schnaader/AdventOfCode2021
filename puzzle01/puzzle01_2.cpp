#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

int main() {
	std::ifstream input("input.txt");

	if (!input.is_open()) {
		printf("Error opening file\n");
		return -1;
	}

	int increase_count = 0;

	// We compare A + B + C      = sum1
	//        and     B + C + D  = sum2
	//
	//    sum2 > sum1
	// => (sum2 - (B + C)) > (sum1 - (B + C))
	// => D > A
	//
	// so we have to compare the current one to the one three numbers before

	std::string line;
	int three_before = -1;
	int two_before = -1;
	int one_before = -1;
	while (getline(input, line)) {
		int this_one = atoi(line.c_str());
		if ((this_one > three_before) && (three_before > -1)) {
			increase_count++;
		}
		
		three_before = two_before;
		two_before = one_before;
		one_before = this_one;
	}
	input.close();

	printf("increase count: %i\n", increase_count);

	return 0;
}
