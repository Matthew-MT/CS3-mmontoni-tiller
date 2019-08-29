#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <vector>
#include <string>

using namespace std;

vector<int> filter(vector<int> values);

int main() {
    int numPieces;
    vector<int> input;
    cin >> numPieces;
    for (int i = 0; i < numPieces; i++) {
        int temp;
        cin >> temp;
        input.push_back(temp);
    }
    vector<int> returned = filter(input);
    cout << returned.at(0) << " " << returned.at(1);
    return 0;
}

vector<int> filter(vector<int> values) {
    sort(values.data(), values.data() + values.size());
    vector<int> output{0, 0};
    for (int i = 0; i < values.size(); i++) {
        output.data()[i % 2] += values.at(i);
    }
    sort(output.data(), output.data() + output.size());
    reverse(output.data(), output.data() + output.size());
    return output;
}