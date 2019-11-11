#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <stack>

using namespace std;

template<bool undirected = true> class Graph {
private:
    unordered_set<size_t> nodes;
    unordered_map<size_t, unordered_map<size_t, int>> edges; // A sort of adjacency list, but with maps
public:
    Graph() {}

    /**
     * Join 2 nodes.
     * Takes advantage of the fact that insertion and deletion are constant-time functions,
     * for std::unordered_set and std::unordered_map.
     */
    void join(size_t a, size_t b, int w = 1) {
        nodes.insert(a);
        nodes.insert(b);
        if (edges.count(a) == 0u) edges.insert({a, {{b, w}}});
        else edges.at(a).insert({b, w});
        if (undirected) {
            if (edges.count(b) == 0u) edges.insert({b, {{a, w}}});
            else edges.at(b).insert({a, w});
        }
    }

    /**
     * Separate 2 nodes.
     */
    void separate(size_t a, size_t b) {
        if (edges.count(a) != 0u) edges.at(a).erase(b);
        if (undirected && edges.count(b) != 0u) edges.at(b).erase(a);
    }

    /**
     * Insert a node.
     */
    void insert(size_t a) {nodes.insert(a);}

    /**
     * Erase a node.
     */
    void erase(size_t a) {
        nodes.erase(a);
        edges.erase(a);
        for (unordered_set<size_t>& s : edges) s.erase(a);
    }

    /**
     * Implementation of DFS.
     */
    bool path(size_t a, size_t b) {
        if (nodes.count(a) == 0u) return false;
        stack<size_t> s;
        unordered_set<size_t> v;
        size_t t;
        s.push(a);
        v.insert(a);
        while (!s.empty()) if (edges.count(t = s.top()) != 0u) {
            s.pop();
            for (auto& e : edges.at(t)) {
                size_t f = e.first;
                if (f == b) return true;
                if (v.insert(f).second) s.push(f);
            }
        }
        return false;
    }

    /**
     * Note:
     * The above function, a DFS implementation, is too slow for the purposes of the assignment.
     * Instead, this function is an "all at once" DFS implementation, that gets all the nodes connected to r and returns them.
     */
    unordered_set<size_t> allConnected(size_t r) {
        if (nodes.count(r) == 0u) return {};
        unordered_set<size_t> o;
        stack<size_t> s;
        size_t t = r;
        s.push(t);

        while (!s.empty()) {
            t = s.top();
            s.pop();
            // Note: std::unordered_set::insert returns whether the element exists when called
            for (auto& e : edges.at(t)) if (o.insert(e.first).second) s.push(e.first);
        }
        return move(o); // Make sure not to copy the list of elements when returning
    }
};

#endif