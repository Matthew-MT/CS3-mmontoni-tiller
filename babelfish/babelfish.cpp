#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <vector>
#include <string>
#include <regex>
#include <map>

using namespace std;

const regex splitter("(\S+)\s+(\S+)");

map<string, string> getInputs(map<string, string> dictionary);
string splitSpace(string input);

int main() {
    map<string, string> dictionary;
    getInputs(dictionary);
}

map<string, string> getInputs(map<string, string> dictionary) {
    string line = "";
    do {
        getline(cin, line);
    } while (line != "");
}

string splitSpace(string input) {
}