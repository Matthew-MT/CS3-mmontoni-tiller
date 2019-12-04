#include "union_find.hpp"
#include <algorithm>

using namespace std;

int main() {
    size_t vertices, edges, a, b, w;
    while (cin >> vertices >> edges && !(vertices == 0u && edges == 0u)) {
        CS3::Graph myGraph(vertices, edges);
        for (size_t i = 0u; i < edges && cin >> a >> b >> w; i++) myGraph.join(a, b, w);
        vector<CS3::sPair> MST;
        int weight = CS3::ExtractMST(myGraph, MST);
        if (MST.size() == 0u || MST.size() != vertices - 1) cout << "Impossible\n";
        else {
            sort(MST.begin(), MST.end());
            cout << weight << '\n';
            for (CS3::sPair& e : MST) cout << e.first << ' ' << e.second << '\n';
        }
    }
}