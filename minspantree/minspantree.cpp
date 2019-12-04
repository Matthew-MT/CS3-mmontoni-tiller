#include "unionfind.hpp"
#include <cassert>
#include <string>

using namespace std;

void compute();
void test();

int main(int argCount, char* args[]) {
    if (argCount > 1 && (string)args[1] == "test") test();
    else compute();
}

void compute() {
    size_t vertices, edges, a, b, w;
    while (cin >> vertices >> edges && !(vertices == 0u && edges == 0u)) {
        Graph myGraph(vertices, edges);
        for (std::size_t i = 0u; i < edges && cin >> a >> b >> w; i++) myGraph.addEdge(a, b, w);
        vector<iPair> MST;
        int weight = KruskalMST(myGraph, MST);
        /**
         * If the generated MST doesn't follow normal MST rules,
         * then it's not possible to generate a proper MST for the given data.
         */
        if (MST.size() == 0u || MST.size() != vertices - 1) cout << "Impossible\n";
        else {
            sort(MST.begin(), MST.end());
            cout << weight << '\n';
            for (iPair& e : MST) cout << e.first << ' ' << e.second << '\n';
        }
    }
}

void test() {
    Graph myGraph(7, 3);
    myGraph.addEdge(0, 3, 1);
    myGraph.addEdge(1, 2, 2);
    myGraph.addEdge(3, 2, 0);

    vector<iPair> MST;
    int weight = KruskalMST(myGraph, MST);

    assert(weight == 3);
    
    sort(MST.begin(), MST.end());
    assert(MST.at(0).first == 0 && MST.at(0).second == 3);
    assert(MST.at(1).first == 1 && MST.at(1).second == 2);
    assert(MST.at(2).first == 2 && MST.at(2).second == 3);
    cout << "All test cases passed.\n";
    string end;
    cin >> end;
}