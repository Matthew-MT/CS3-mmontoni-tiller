/**
 * Date: 09/20/19
 * Author: Matthew M-T
 * Info:
 * Compiled and tested in g++ MinGW v8.2.0
 */

#include <iostream>
#include <cassert>
#include <cmath>

using namespace std;

/**
 * Function declarations
 */
auto modifier(const unsigned long long& type);
void test();
void compute();
bool check(const unsigned long long& m, const unsigned long long& n, const unsigned long long& t);
bool fact(const unsigned long long& input, const unsigned long long& max);
unsigned long long ceiling(const long double& input);

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
 * Returns a ready-made lambda function to test numbers on
 */
auto modifier(const unsigned long long& type) {
    return [type](const unsigned long long& input, const unsigned long long& max) -> bool {
        if (type == 1) return fact(input, max);
        else if (type == 2) return pow(2, input) <= max;
        else if (type >= 3 && type <= 5) return pow(input, 7 - type) <= max;
        else if (type == 6) return ceiling((long double)input * log2(input)) <= max;
        else if (type == 7) return input <= max;
        else throw "Invalid type";
    };
}

/**
 * 3 independent test cases to ensure the algorithm is correct
 */
void test() {
    unsigned long long m = 100000, n = 200, t = 4;
    assert(!check(m, n, t));
    m = 20000, n = 100, t = 5;
    assert(check(m, n, t));
    m = 100000, n = 50, t = 2;
    assert(!check(m, n, t));
    cout << "All test cases passed.";
    cin.get();
}

/**
 * Takes input and returns "AC" if the numbers pass, or "TLE" if the given program stats will fail
 */
void compute() {
    // 1: input m, n, & t
    unsigned long long m, n, t;
    while (cin >> m >> n >> t) cout << (check(m, n, t) ? "AC" : "TLE") << '\n';
}

/**
 * Check input for whether the program successfully runs in under the number of operations given
 */
bool check(const unsigned long long& m, const unsigned long long& n, const unsigned long long& t) {
    // 2: find the function that determines growth rate
    auto succeeded = modifier(t);
    // 3: return whether the program runs within the number of operations given
    return succeeded(n, m);
}

/**
 * Intelligent factorial function that stops when it is clear the given number won't pass
 */
bool fact(const unsigned long long& input, const unsigned long long& max) {
    if (input < 0) throw "Invalid input";
    else if (input == 1 || input == 2) return input <= max;
    else {
        unsigned long long factorial = input;
        for (unsigned long long i = input; i > 2; i--) {
            if ((long double)max / (long double)factorial < 1.0) return false;
            else factorial *= (i - 1);
        }
        if ((long double)max / (long double)factorial < 1.0) return false;
        else return true;
    }
}

/**
 * Returns the ceiling of the given floating point
 */
unsigned long long ceiling(const long double& input) {
    unsigned long long floored = floor(input);
    if ((long double)input == (long double)floored) return floored;
    else return floored + 1;
}