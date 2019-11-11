#include "graph.hpp"
#include <string>
#include <sstream>
#include <time.h>

using namespace std;

int main() {
    stringstream stream;
    srand(time(NULL));
    int a = rand() % 2000, b = rand() % a;
    stream << a << ' ' << b << '\n';
    for (int i = 0; i < b; i++) stream << rand() % a << ' ' << rand() % a << '\n';

    size_t houses, cables, parent, connected;
    stream >> houses >> cables;
    Graph myGraph(houses);
    //for (int i = 1; i <= houses; i++) myGraph.insert(i);
    for (int i = 0; i < cables && stream >> parent >> connected; i++) {
        cout << parent << ' ' << connected << '\n';
        myGraph.addEdge(parent - 1, connected - 1);
        myGraph.addEdge(connected - 1, parent - 1);
    }
    string output = "";
    for (size_t i = 1; i < houses; i++) if (!DFS<Graph>(myGraph, 0, i)) output += to_string(i + 1) + '\n';
    cout << (output != "" ? output : "Connected\n");
    string end;
    cin >> end;
    return 0;
}