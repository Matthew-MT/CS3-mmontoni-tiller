/**
 * Date: 09/11/19
 * Author: Matthew M-T
 * Info:
 * Compiled & tested in g++ v9.2.1
 */

#include <iostream>
#include <cassert>
#include <vector>
#include <string>

using namespace std;

/**
 * Function declarations
 */
void test();
void compute();
int overallMax(const int* begin, const int* end, const int& max);
int findMax(const int* begin, const int* end, const int& max);

/**
 * Main execution, decides whether to call test() or compute() based on user input
 */
int main(int argCount, char* args[]) {
    if (argCount > 1) {
        string arg = args[1];
        if (arg == "test") test();
        else compute();
    } else compute();
}

/**
 * 3 independent test cases to ensure the algorithm is correct
 */
void test() {
    vector<int> assertion {1, 3, 4, 3, 5, 2, 8};
    assert(overallMax(assertion.data(), assertion.data() + assertion.size(), 4) == 2);
    assert(overallMax(assertion.data(), assertion.data() + assertion.size(), 6) == 3);
    assert(overallMax(assertion.data(), assertion.data() + assertion.size(), 10) == 4);
    cout << "All test cases passed.";
}

/**
 * Function that takes inputs and then cout's how many total different values may be used in linear sequence to reach the given max
 */
void compute() {
    int num, max, input;
    vector<int> inputs;
    while (cin >> num >> max) { // If no more input, end the loop
        for (int i = 0; i < num; i++) {
            if (!(cin >> input)) return;
            inputs.push_back(input); // Get values to check based on given ranges
        }
        cout << overallMax(inputs.data(), inputs.data() + inputs.size(), max) << '\n'; // Check values
        inputs.clear();
    }
}

/**
 * Actual algorithm to determine the number of different values can be added in linear sequence until the max is reached
 */
int overallMax(const int* begin, const int* end, const int& max) { // Range: [begin, end)
    vector<int> results;
    for (int i = 0; i < abs(end - begin) && i > -1 * abs(end - begin); i += (begin > end ? -1 : 1)) { // Note: allows direction reversal
        results.push_back(findMax(begin + i, end, max)); // Checks the subsequence starting at (begin + i) and ending at (end - 1)
    }
    int highest = results.at(0); // Logically, highest value must be a value included in the set
    for (const int& val : results) if (val > highest) highest = val; // Finds highest value
    return highest;
}

int findMax(const int* begin, const int* end, const int& max) { // Starts with the first integer in the range and continues to add to total
    // until max is reached
    int output = 0, total = 0;
    for (int i = 0; i < abs(end - begin) && i > -1 * abs(end - begin); i += (begin > end ? -1 : 1)) {
        if (total + *(begin + i) <= max) total += *(begin + i), output++; // Checks whether the current integer is able to be added
    }
    return output;
}