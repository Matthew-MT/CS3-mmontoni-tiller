/**
 * Date: 09/09/19
 * Author: Matthew M-T
 * Info:
 * Compiled & tested in g++ v9.2.1
 */

#include <iostream>
#include <vector>
#include <string>
#include <cassert>

using namespace std;

void test();
void compute();
void getInputs(pair<vector<int>, vector<int>>&, const int&, const int&);
int countDupes(pair<vector<int>, vector<int>>&);

int main(int argCount, char* args[]) {
    if (argCount > 1) {
        string arg = args[1];
        if (arg == "test") test();
        else compute();
    } else compute();
    return 0;
}

void test() {
    pair<vector<int>, vector<int>> assert1 = {
        vector<int> {1, 2, 4, 6},
        vector<int> {2, 3, 4}
    };
    assert(countDupes(assert1) == 2);
    pair<vector<int>, vector<int>> assert2 = {
        vector<int> {3, 5, 6, 10},
        vector<int> {4, 6, 7, 8, 9}
    };
    assert(countDupes(assert2) == 1);
    pair<vector<int>, vector<int>> assert3 = {
        vector<int> {2, 4, 6, 8, 10, 12},
        vector<int> {5, 6, 7, 8, 9, 10}
    };
    assert(countDupes(assert3) == 3);
    cout << "All test cases passed.";
}

void compute() {
    int a, b;
    pair<vector<int>, vector<int>> inputs;
    cin >> a >> b;
    getInputs(inputs, a, b);
    cin >> a >> b;
    if (a == 0 && b == 0) cout << countDupes(inputs);
}

void getInputs(pair<vector<int>, vector<int>>& inputs, const int& a, const int& b) {
    int input;
    for (int i = 0; i < a; i++) {
        cin >> input;
        inputs.first.push_back(input);
    }
    for (int i = 0; i < b; i++) {
        cin >> input;
        inputs.second.push_back(input);
    }
}

int countDupes(pair<vector<int>, vector<int>>& inputs) {
    int both = 0;
    for (int i = 0, j = 0; i < inputs.first.size() && j < inputs.second.size(); i++) {
        if (inputs.first[i] > inputs.second[j]) {
            for (; j < inputs.second.size() && inputs.first[i] > inputs.second[j]; j++) {}
        }
        if (inputs.first[i] == inputs.second[j]) j++, both++;
        if (j >= inputs.second.size()) break;
    }
    return both;
}