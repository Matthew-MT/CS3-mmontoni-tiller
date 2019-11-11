#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <unordered_map>
#include <functional>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
#include <stack>

namespace CS3 {
    template<class DataType = int, bool undirected = true> class Graph {
    public:
        enum Behavior {setCur, joinCur, none};
    private:
        std::vector<DataType> nodes;
        std::vector<std::vector<int>> adjMatrix;
        DataType* current = nullptr;
        bool hasData = false;

        bool hasConnection(std::size_t cur, std::size_t con, std::vector<bool>& stack) {
            if (cur == con) return true;
            for (int i = 0; i < adjMatrix.at(cur).size(); i++) if (adjMatrix.at(cur).at(i) != 0 && !stack.at(i)) {
                stack.at(i) = true;
                bool result = this->hasConnection(i, con, stack);
                if (result) return result;
            }
            return false;
        }
    public:
        Graph() {}

        bool find(DataType toBeFound, bool setCurrent = true) {
            if (!hasData) return false;
            std::size_t found = *(std::find(nodes.begin(), nodes.end(), toBeFound)) - *(nodes.begin());
            if (found != nodes.size()) {
                if (setCurrent) current = &(nodes.at(found));
                return true;
            } else return false;
        }

        void insert(DataType toBeInserted, Behavior then = none, int weight = 1) {
            nodes.push_back(toBeInserted);
            for (std::vector<int>& v : adjMatrix) v.push_back(0);
            adjMatrix.push_back(std::vector<int>(nodes.size(), 0));
            if (!hasData || then == setCur) {
                hasData = true;
                current = &(nodes.at(0));
            } else if (then == joinCur) this->join(toBeInserted, weight);
        }

        bool erase(DataType toBeErased) {
            std::size_t found = std::find(nodes.begin(), nodes.end(), toBeErased) - nodes.begin();
            
            if (found != nodes.size()) {
                adjMatrix.erase(adjMatrix.begin() + found);
                for (std::vector<int>& v : adjMatrix) v.erase(v.begin() + found);

                if (current == nodes.at(found)) current = nullptr;
                nodes.erase(nodes.begin() + found);
                if (nodes.size() == 0) hasData = false;
                return true;
            } else return false;
        }

        bool join(DataType joined, int weight = 1) {
            if (current == nullptr) return false;
            else {
                std::size_t found = *(std::find(nodes.begin(), nodes.end(), joined)) - *(nodes.begin());
                if (found != nodes.size()) {
                    std::vector<int>& index = adjMatrix.at(found);
                    std::size_t currentPlace = *(std::find(nodes.begin(), nodes.end(), *current)) - *(nodes.begin());
                    index.at(currentPlace) = weight;
                    if (undirected) {
                        std::vector<int>& unIndex = adjMatrix.at(currentPlace);
                        unIndex.at(found) = weight;
                    }
                    return true;
                } else return false;
            }
        }

        bool separate(DataType separated) {
            if (current == nullptr) return false;
            else {
                std::size_t found = *(std::find(nodes.begin(), nodes.end(), separated)) - *(nodes.begin());
                if (found != nodes.size()) {
                    std::vector<int>& index = adjMatrix.at(found);
                    std::size_t currentPlace = *(std::find(nodes.begin(), nodes.end(), *current)) - *(nodes.begin());
                    index.at(currentPlace) = 0;
                    if (undirected) {
                        std::vector<int>& unIndex = adjMatrix.at(currentPlace);
                        unIndex.at(found) = 0;
                    }
                    return true;
                } else return false;
            }
        }

        bool connected(DataType connection) {
            if (current == nullptr) return false;
            else {
                std::size_t
                    cur = *(std::find(nodes.begin(), nodes.end(), *current)) - *(nodes.begin()),
                    con = *(std::find(nodes.begin(), nodes.end(), connection)) - *(nodes.begin());
                std::vector<bool> stack(nodes.size(), false);
                return this->hasConnection(cur, con, stack);
            }
        }

        std::unordered_map<DataType, int> neighbors(DataType node) {
            std::unordered_map<DataType, int> edges;
            int found = *(std::find(nodes.begin(), nodes.end(), node)) - *(nodes.begin());
            if (found != nodes.size()) {
                std::vector<int>& weights = adjMatrix.at(found);
                for (int i = 0, w; i < weights.size(); i++, w = weights.at(i)) if (w != 0) edges.insert({nodes.at(i), w});
            }
            return edges;
        }
    };
};

#endif