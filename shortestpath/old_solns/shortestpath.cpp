#include "graph.hpp"
#include <sstream>
#include <string>
#include <regex>
#include <time.h>
#include <ctime>

using namespace std;

stringstream test;

void getInputs(int* inputs) {
    string line, gotten = "";
    size_t place = 0u;
    getline(test, line);
    for (char c : line) {
        if (c == ' ' || c == '\n') {
            inputs[place++] = atoi(gotten.c_str());
            gotten = "";
        } else gotten += c;
    }
    if (gotten != "") inputs[place] = atoi(gotten.c_str());
}

int main() {
    //ios_base::sync_with_stdio(false);

    Graph<false> myGraph;
    /*size_t n = 10000u, m = 30000u, q = 100u, s = 0u;
    srand(time(NULL));
    for (size_t i = 0u; i < n; i++) myGraph.insert(i);
    for (size_t i = 0u; i < m; i++) myGraph.join(rand() % n, rand() % n, rand() % 1000);
    clock_t begin = clock();
    unordered_map<size_t, Graph<false>::Dist> res = myGraph.shortestPaths(s);
    cout << "Duration for max size: " << (clock() - begin) / ((double)CLOCKS_PER_SEC) << '\n';

    string finish;
    cin >> finish;
    return 0;*/

    int* data = new int[4];
    //cout << "Begin\n";
    size_t nodes, edges, queries, start, parent, connected, query;
    int weight;
    unordered_map<size_t, Dist> distances;
    string output = "";
    while (true) {
        cin >> nodes >> edges >> queries >> start;
        if (nodes == 0u && edges == 0u && queries == 0u && start == 0u) break;
        myGraph = Graph<false>{};
        for (size_t i = 0u; i < nodes; i++) myGraph.insert(i);

        for (size_t i = 0u; i < edges && cin >> parent >> connected >> weight; i++) myGraph.join(parent, connected, weight);
        distances = myGraph.shortestPaths(start);

        for (size_t i = 0u; i < queries && cin >> query; i++) {
            Dist distance = distances.at(query);
            if (distance.inf) output += "Impossible\n";
            else output += to_string(distance.val) + '\n';
        }
        output += '\n';
    }
    cout << output;
    delete[] data;

    /*string end;
    cin >> end;*/
}