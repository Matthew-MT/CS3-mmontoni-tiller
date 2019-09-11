/**
 * Date: 09/09/19
 * Author: Matthew M-T
 * Info:
 * Compiled & tested in g++ v9.2.1
 */

#include <algorithm>
#include <iostream>
#include <cassert>
#include <vector>
#include <string>

using namespace std;

void test();
void compute();
void getInputs(vector<int>& inputs, const int& num);
int countDupes(vector<int>& inputs, const int& num);
bool findCD(vector<int>& CDs, const int& CD);

int main(int argCount, char* args[]) {
    if (argCount > 1) {
        string arg = args[1];
        if (arg == "test") test();
        else compute();
    } else compute();
    return 0;
}

void test() {
    vector<int> assertion {1, 2, 4, 6, 7, 8, 10, 14, 15, 16};
    assert(findCD(assertion, 4));
    assert(!findCD(assertion, 9));
    assert(findCD(assertion, 14));
    cout << "All test cases passed.";
    return;
}

void compute() {
    int a, b;
    do {
        if (!(cin >> a >> b)) return;
        if (a != 0 && b != 0) {
            vector<int> inputs;
            getInputs(inputs, a);
            sort(inputs.begin(), inputs.end());
            cout << countDupes(inputs, b) << '\n';
        } else break;
    } while (a != 0 && b != 0);
    return;
}

void getInputs(vector<int>& inputs, const int& num) {
    int input;
    for (int i = 0; i < num && cin >> input; i++) inputs.push_back(input);
}

int countDupes(vector<int>& inputs, const int& num) {
    int input, both = 0;
    for (int i = 0; i < num && cin >> input; i++) if (findCD(inputs, input)) both++;
    return both;
}

bool findCD(vector<int>& CDs, const int& CD) {
    return binary_search(CDs.begin(), CDs.end(), CD);
}