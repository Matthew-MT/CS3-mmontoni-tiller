#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <stack>
#include <queue>

using namespace std;

class Dist {
public:
    int val;
    bool inf = false;
    Dist(int init) {val = init;}
    Dist(bool init) {inf = true;}

    void set(int v) {
        inf = false;
        val = v;
    }
};

template<bool undirected = true> 
class Graph {
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
        if (nodes.count(r) == 0u || edges.count(r) == 0u) return {};
        unordered_set<size_t> c;
        stack<size_t> s;
        size_t t;
        s.push(t);

        while (!s.empty()) {
            t = s.top();
            s.pop();
            // Note: std::unordered_set::insert returns whether the element exists when called
            for (auto& e : edges.at(t)) if (c.insert(e.first).second) s.push(e.first);
        }
        return move(c); // Make sure not to copy the list of elements when returning
    }

    /**
     * SSSP algorithm. Uses stack-based implementation.
     */
    /*unordered_map<size_t, Dist> shortestPaths(size_t r) {
        unordered_map<size_t, Dist> d;
        for (size_t n : nodes) d.insert({n, {true}});
        if (nodes.count(r) == 0u || edges.count(r) == 0u) return move(d);
        d.at(r).set(0u);
        stack<size_t> s;
        s.push(r);
        size_t t;

        while (!s.empty()) {
            t = s.top();
            s.pop();
            if (edges.count(t) != 0u) for (auto& e : edges.at(t)) {
                size_t f = e.first, w = d.at(t).val + e.second; // The currently computed weight is the weight of the current root,
                // plus the weight of the current edge
                // Note: nodes with a value of "inf" that can't be reached will never reach this step
                if (d.at(f).inf || d.at(f).val > w) {
                    d.at(f).set(w); // If this weight is smaller, replace the currently held weight
                    s.push(f); // If the weight changes, the node should be put (back) on the stack
                }
            }
        }
        return move(d);
    }*/

    /**
     * SSSP algorithm. Uses queue-based implementation.
     */
    unordered_map<size_t, Dist> shortestPaths(size_t r) {
        unordered_map<size_t, Dist> dist;
        using ip = pair<int, size_t>;
        priority_queue<ip, vector<ip>, greater<ip>> queue;

        for (size_t n : nodes) dist.insert({n, {true}});
        if (nodes.count(r) == 0u || edges.count(r) == 0u) return move(dist);
        vector<bool> visited(nodes.size(), false);

        dist.at(r).set(0u);
        queue.push({0, r});

        size_t u;

        while (!queue.empty()) {
            u = queue.top().second;
            queue.pop();

            if (visited.at(u)) continue;
            visited.at(u) = true;

            if (edges.count(u) != 0u) for (auto& p : edges.at(u)) { // For each neighbor
                size_t v = p.first;
                if (visited.at(v)) continue;

                int w = p.second + dist.at(u).val;
                // The currently computed weight is the weight of the current root,
                // plus the weight of the current edge
                // Note: nodes with a value of "inf" that can't be reached will never reach this step

                if (dist.at(v).inf || dist.at(v).val > w) {
                    dist.at(v).set(w); // If this weight is smaller, replace the currently held weight
                    queue.push({w, v});
                    // If the weight changes, the node should be put (back) on the stack
                }
            }
        }
        return move(dist);
    }

    void clear() {
        nodes.clear();
        for (auto& list : edges) list.second.clear();
        edges.clear();
    }
};

#endif