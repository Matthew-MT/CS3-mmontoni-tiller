#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <vector>
#include <string>

using namespace std;

void test();
void compute();
vector<int> filter(vector<int> values);

int main(int argCount, char* args[]) {
    if (argCount > 1) {
        string arg = args[1];
        if (arg == "test") test();
    } else compute();

    return 0;
}

void test() {
    vector<int> assert1 = filter(vector<int> {5, 3, 8, 1});
    assert(assert1.at(0) == 11 && assert1.at(1) == 6);
    vector<int> assert2 = filter(vector<int> {7, 3, 9, 4, 8, 3});
    assert(assert2.at(0) == 19 && assert2.at(1) == 15);
    vector<int> assert3 = filter(vector<int> {4, 16, 3, 7, 19});
    assert(assert3.at(0) == 29 && assert3.at(1) == 20);
    cout << "All test cases passed.";

    return;
}

void compute() {
    int numPieces;
    vector<int> input{};
    cin >> numPieces;

    for (int i = 0; i < numPieces; i++) {
        int temp;
        cin >> temp;
        input.push_back(temp);
    }
    vector<int> returned = filter(input);

    cout << returned.at(0) << " " << returned.at(1);

    return;
}

vector<int> filter(vector<int> values) {
    sort(values.begin(), values.end());

    vector<int> output{0, 0};
    for (int i = 0; i < values.size(); i++) output.data()[i % 2] += values.at(i);

    sort(output.begin(), output.end());
    reverse(output.begin(), output.end());

    return output;
}