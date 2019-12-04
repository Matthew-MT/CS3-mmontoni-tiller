#include <iostream>
#include <vector>
#include <queue> // priority_queue
#include <climits> // sizes of integral types
#include <sstream>
#include <list>
#include <algorithm>

using namespace std;

using iPair = pair<int, int>;

// a structure to represent a weighted edge in graph 
struct Edge 
{ 
    int src, dest, weight; 
    // for min priority queue 
     bool operator<(const Edge &other) const {
        return this->weight > other.weight;
    }
};

// a structure to represent undirected 
// and weighted graph 
struct Graph 
{ 
    // V -> Number of vertices, E -> Number of edges 
    int V, E; 
    // graph is stored in a min heap priority_queue
    // Kruskal algo requires working with edges with smallest to highest weight 
    priority_queue<Edge, vector<Edge> > edges;
    // constructor
    Graph(int v, int e) {
        V = v;
        E = e;
    }
    
    void addEdge(int u, int v, int w) {
        edges.push({u, v, w});
    }
};

