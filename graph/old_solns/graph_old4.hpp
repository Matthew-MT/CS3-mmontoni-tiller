#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_set>
#include <list>
#include <stack>

using namespace std;

class Graph {
private:
    vector<vector<int> > matrix;
    size_t numEdge;
    unordered_set<int> nodes;

public:
    Graph(size_t n) {
        this->numEdge = 0;
        // initialize matrix with 0x
        for (int i=0; i<n; i++) {
            vector<int> v(n, 0); // create an array of n elements initialized with 0s
            matrix.push_back(v);
        }
    }
    // return the number of vertices/nodes
    size_t nodeCount() {
        return nodes.size();
    }

    // return the current number of edges
    size_t edgeCount() {
        return numEdge;
    }
    
    // add a new edge from node u to node v, with weight w
    void addEdge(int u, int v, int w=1) {
        if (w == 0) return;
        matrix[u][v] = w;
        ++numEdge;
        nodes.insert(u);
        nodes.insert(v);
    }

    // get the weight value for an edge, u->v
    int weight(int u, int v) {
        return matrix[u][v];
    }

    // removes an edge from the graph
    void removeEdge(int u, int v) {
        matrix[u][v] = 0;
        --numEdge;
    }

    // check if there's an edege between u and v
    bool hasEdge(int u, int v) {
        return matrix[u][v] != 0;
    }
    
    // returns vector containing neighbors of u
    vector<int> neighbors(int u) {
        vector<int> neighs;
        for(int v = 0; v<matrix[u].size(); v++) {
            if (matrix[u][v] != 0) neighs.push_back(v);
        }
        return neighs;
    }

    // print graph
    void print() {
        for(auto u: matrix) {
            for (auto v: u)
                cout << v << " ";
            cout << endl;
        }
    }
};



// iterative version using stack
template<class T>
bool DFS(T &G, int start, int searched) {
    vector<bool> visited(G.nodeCount(), false); // boolean vector to keep track of visited nodes
    stack<int> vertices;
    vertices.push(start);
    while(!vertices.empty()) {
        int u = vertices.top(); // get the top of the stack
        // pop the stack
        vertices.pop();
        // mark node as visited
        // do something with the data if not already visited
        // stack may contain same vertex twice; print the item if it's not already visited
        if (!visited[u]) {
            if (searched == u) return true;
            visited[u] = true;
        }
        // add all the unvisited neighbors (adjacent vertices) of u to the stack
        for(auto v: G.neighbors(u)) {
            if (!visited[v])
                vertices.push(v);
        }
    }
    return false;
}

#endif