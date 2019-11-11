/**
 * Date: 10/10/19
 * Author: Matthew M-T
 * Info:
 * Compiled & tested in g++ v9.2.1
 */

#include "graph.hpp"
#include <cassert>
#include <string>

using namespace std;

void test();
void compute();

int main(int argCount, char* args[]) {
    if (argCount > 1) {
        string arg = args[1];
        if (arg == "test") test();
        else compute();
    } else compute();
    return 0;
}

void test() {
    Graph<> myGraph;
    myGraph.join(1u, 3u);
    myGraph.join(2u, 5u);
    myGraph.join(1u, 4u);
    myGraph.join(5u, 7u);
    myGraph.join(3u, 4u);

    assert(myGraph.allConnected(1u).count(5u) == 0u); // 1 is not connected to 5 at any point

    assert(myGraph.allConnected(2u).count(7u) == 1u); // 2 is connected to 7 at some point

    assert(myGraph.allConnected(3u).count(4u) == 1u); // 3 is connected to 4 at some point

    cout << "All test cases passed.\n";
    string end;
    cin >> end;
}

void compute() {
    ios_base::sync_with_stdio(false); // *shrug*
    Graph<> myGraph;
    size_t houses, cables, parent, connected;
    cin >> houses >> cables;
    for (size_t i = 1; i <= cables && cin >> parent >> connected; i++) myGraph.join(parent, connected); // Input all edges

    unordered_set<size_t> WiFi = myGraph.allConnected(1u); // Get all nodes connected to root (1)
    string output = "";
    for (size_t i = 2; i <= houses; i++) if (WiFi.count(i) == 0) output += to_string(i) + '\n'; // If a node is not connected to 1, add it to list
    cout << (output != "" ? output : "Connected\n");
}