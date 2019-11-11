/**
 * Brute force solution, to prove that it is solvable.
 */

#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <vector>
#include <set>

using namespace std;

int main() {
    size_t houses, cables, parent, connected;
    unordered_set<size_t> map;
    unordered_map<size_t, unordered_set<size_t>> connections;
    map.insert(1u);

    cin >> houses >> cables;
    for (size_t i = 1; i <= cables && cin >> parent >> connected; i++) {
        if (map.count(parent) != 0u) {
            map.insert(connected);
            continue;
        }
        if (map.count(connected) != 0u) {
            map.insert(parent);
            continue;
        }
        if (connections.count(parent) == 0u) connections.insert({parent, {connected}});
        else connections.at(parent).insert(connected);
    }
    
    bool end = false;
    while (!connections.empty() && !end) {
        end = true;
        for (size_t i = 2; i <= houses; i++) if (connections.count(i) != 0u && map.count(i) != 0u) {
            for (size_t s : connections.at(i)) {
                end = false;
                map.insert(s);
            }
            connections.erase(i);
        }
    }

    set<size_t> sorted(map.begin(), map.end());
    string output = "";
    for (size_t i = 2; i <= houses; i++) if (sorted.count(i) == 0u) output += to_string(i) + '\n';
    cout << (output != "" ? output : "Connected\n");

    string stop;
    cin >> stop;
    return 0;
}