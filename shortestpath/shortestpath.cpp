/**
 * Date: 11/17/19
 * Author: Matthew M-T
 * Info:
 * Compiled & tested in MinGW v8.3.0
 */

#include "graph.hpp"
#include <iostream>
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
    Graph myGraph(10u);

    myGraph.addEdge(0, 1, 1);
    myGraph.addEdge(1, 2, 2);
    myGraph.addEdge(2, 4, 1);
    myGraph.addEdge(4, 3, 1);
    myGraph.addEdge(2, 3, 3);
    myGraph.addEdge(5, 7, 2);
    myGraph.addEdge(3, 6, 2);
    myGraph.addEdge(1, 6, 7);
    myGraph.addEdge(9, 8, 4);
    myGraph.addEdge(9, 7, 3);

    vector<int> dist;
    Dijkstra<Graph>(myGraph, 0u, dist);

    assert(dist[2] == 3);
    assert(dist[7] == INT_MAX);
    assert(dist[6] == 7);

    cout << "All test cases passed.\n";
    string end;
    cin >> end;
}

void compute() {
    int
        n, // Number of nodes
        m, // Number of edges
        q, // Number of queries
        s, // Starting node
        u, // Edge from u
        v, // to v
        w, // with weight w
        a; // Query

    while (cin >> n >> m >> q >> s && !(n == 0 && m == 0 && q == 0 && s == 0)) {
        Graph myGraph((size_t)n);
        for (int i = 0; i < m && cin >> u >> v >> w; i++) myGraph.addEdge(u, v, w);
        vector<int> dist;
        Dijkstra<Graph>(myGraph, s, dist);
        for (int i = 0; i < q && cin >> a; i++) cout << (dist[a] != INT_MAX ? to_string(dist[a]) + '\n' : "Impossible\n");
        cout << '\n';
    }
}