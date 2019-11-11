#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <algorithm>
#include <iostream>
#include <vector>

namespace CS3 {
    class Graph {
    private:
        std::size_t edges;
        std::vector<std::size_t> nodes;
        std::vector<std::vector<int>> adjMatrix;

        bool hasPath(std::size_t a, std::size_t b, std::vector<bool>& stack) {
            if (a == b) return true;
            else if (!stack.at(a)) {
                stack.at(a) = true;
                for (int i = 0; i < nodes.size(); i++) if (i != a && this->weight(a, i) != 0) {
                    bool res = this->hasPath(i, b, stack);
                    if (res) return res;
                }
            }
            return false;
        }
    public:
        Graph() {}

        std::size_t nodeCount() {return nodes.size();}
        std::size_t edgeCount() {return edges;}

        void insert(std::size_t added) {
            //if (std::find(nodes.begin(), nodes.end(), added) == nodes.end()) {
                nodes.push_back(added);
                for (std::vector<int>& connections : adjMatrix) connections.push_back(0);
                adjMatrix.push_back(std::vector<int>(nodes.size(), 0));
                //return true;
            //} else return false;
        }
        
        bool erase(std::size_t erased) {
            auto it = std::find(nodes.begin(), nodes.end(), erased);
            auto adjusted = it - nodes.begin();
            std::size_t found = *it - *(nodes.begin());
            if (found != nodes.size()) {
                for (int edge : adjMatrix.at(found)) if (edge != 0) edges--;
                adjMatrix.erase(adjMatrix.begin() + adjusted);
                for (std::vector<int>& connections : adjMatrix) {
                    if (connections.at(found) != 0) edges--;
                    connections.erase(connections.begin() + adjusted);
                }
                return true;
            } else return false;
        }

        bool join(std::size_t a, std::size_t b, int weight = 1) {
            if (a >= nodes.size() || b >= nodes.size()/* || adjMatrix.at(a).at(b) != 0*/) return false;
            else {
                adjMatrix.at(a).at(b) = weight;
                adjMatrix.at(b).at(a) = weight;
                edges++;
                return true;
            }
        }

        bool separate(std::size_t a, std::size_t b) {
            if (a >= nodes.size() || b >= nodes.size()/* || adjMatrix.at(a).at(b) == 0*/) return false;
            else {
                adjMatrix.at(a).at(b) = 0;
                adjMatrix.at(b).at(a) = 0;
                edges--;
                return true;
            }
        }

        std::size_t neighbors(std::size_t n) {
            if (n >= nodes.size()) return 0u;
            std::size_t found = 0u;
            for (int w : adjMatrix.at(n)) if (w != 0) found++;
            return found;
        }

        int weight(std::size_t a, std::size_t b) {
            if (a >= nodes.size() || b >= nodes.size()) return 0;
            else return adjMatrix.at(a).at(b);
        }

        bool connected(std::size_t a, std::size_t b) {
            if (a >= nodes.size() || b >= nodes.size()) return false;
            std::vector<bool> stack(nodes.size(), false);
            return this->hasPath(a, b, stack) != 0;
        }
    };
};

#endif