#ifndef UNION_FIND_HPP
#define UNION_FIND_HPP

#include "graph.hpp"

namespace CS3 {
    using sPair = std::pair<std::size_t, std::size_t>;

    class UnionFind {
    private:
        std::vector<int> nodes;
        std::vector<int> weights;
    public:
        UnionFind(std::size_t size = 0u) {
            nodes.resize(size);
            std::fill(nodes.begin(), nodes.end(), -1);
            weights.resize(size);
            std::fill(weights.begin(), weights.end(), 1);
        }

        int find(std::size_t node) {
            if (nodes.at(node) == -1) return node;
            else return nodes.at(node) = this->find((std::size_t)nodes.at(node));
        }

        void join(std::size_t a, std::size_t b) {
            std::size_t
                rootA = this->find(a),
                rootB = this->find(b);
            
            if (rootA != rootB) {
                if (weights.at(rootA) < weights.at(rootB)) {
                    nodes.at(rootA) = rootB;
                    weights.at(rootB) += weights.at(rootA);
                } else {
                    nodes.at(rootB) = rootA;
                    weights.at(rootA) += weights.at(rootB);
                }
            }
        }
    };

    int ExtractMST(Graph& graph, std::vector<sPair>& MST) {
        if (graph.edgeCount == 0u) return 0;
        std::size_t verticeCount = graph.verticeCount;
        UnionFind tree(verticeCount);
        int weight = 0;
        while (verticeCount > 1u && !graph.edges.empty()) {
            Edge smallest = graph.edges.top();
            graph.edges.pop();
            std::size_t
                a = smallest.src,
                b = smallest.dst,
                ar = tree.find(a),
                br = tree.find(b);

            //std::cout << "Log: " << a << ' ' << b << ' ' << ar << ' ' << br << '\n';

            if (ar != br) {
                weight += smallest.weight;
                if (a > b) std::swap(a, b);
                MST.push_back({a, b});
                tree.join(a, b);
                verticeCount--;
            }
        }
        return weight;
    }
};

#endif