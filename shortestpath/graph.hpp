#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <vector>
#include <queue> // priority_queue
#include <climits> // sizes of integral types
#include <utility> // make_pair
#include <list>
#include <algorithm>

using namespace std;
using iPair = pair<int, int>;

// SSSP
template<class T>
void Dijkstra(T & G, int source, vector<int>& dist) {
    // min priority_queue of vertices that need to be processed
    // stores pair of <weight, vertex>
    priority_queue<iPair, vector<iPair>, greater<iPair> > pq;
    dist.resize(G.nodeCount());
    fill(dist.begin(), dist.end(), INT_MAX);
    vector<bool> visited(G.nodeCount(), false);
    dist[source] = 0;
    pq.push({0, source}); // {weight, vertex}
    while (! pq.empty()) {
        int u = pq.top().second;
        pq.pop();
        // Single Destination:
        // if interested to find the path to one destination
        // check here if u == dest node; break if so
        if (visited[u]) continue;
        visited[u] = true;
        for(auto p: G.neighbors(u)) {
            int v = p.first;
            if (visited[v]) continue;
            int w = p.second;
            // if there's a shorter path to v through u
            int d = dist[u] + w;
            if (d < dist[v]) {
                dist[v] = d;
                pq.push({d, v});
                // update parent vector if path needs to be recorded
                //parent[v] = u;
            }
        }
    }
}

class Graph {
    private:
        vector<list<iPair> > graph; // list stores pair of neighbor id and weight
    
    public:
        Graph(size_t n) {
            for (int i=0; i<n; i++) {
                list<iPair> v; // create an empty list of int, int pair type
                graph.push_back(v);
            }
        }
    
        // return the number of vertices/nodes
        size_t nodeCount() {
            return graph.size();
        }
        
        // add a new edge from node u to node v, with weight w
        // assumes nodes are numbered from 0 to n-1
        void addEdge(int u, int v, int w) {
            graph[u].push_back({v, w});
            // if undirected graph must add edge from v to u
            // graph[v].push_bck({u, w}); 
        }

        // returns list of pairs containing neighbors of u, and weight
        list<iPair> neighbors(int u) {
            return graph[u];
        }
};

#endif