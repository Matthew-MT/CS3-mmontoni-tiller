#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <queue>

namespace CS3 {
    class Edge {
    public:
        Edge(std::size_t s, std::size_t d, int w) {src = s, dst = d, weight = w;}
        std::size_t src, dst;
        int weight;
        
        bool operator<(const Edge& other) const {return weight > other.weight;}
    };

    class Graph {
    public:
        Graph(std::size_t v, std::size_t e) {verticeCount = v, edgeCount = e;}
        std::size_t verticeCount, edgeCount;
        std::priority_queue<Edge, std::vector<Edge>> edges;

        void join(std::size_t a, std::size_t b, int w) {edges.push({a, b, w});}
    };
}

#endif