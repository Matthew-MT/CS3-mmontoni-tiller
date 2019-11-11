#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <unordered_map>
#include <functional>
#include <algorithm>
#include <vector>
#include <memory>

using namespace std;

template<class PtrType> class InternalPtr {
private:
    PtrType* ptr = nullptr;
    bool validPtr = false;
    unsigned int refCount = 0;
public:
    InternalPtr(PtrType* init) {
        ptr = init;
        validPtr = true;
    }

    ~InternalPtr() {delete ptr;}

    void set(PtrType* now) {ptr = now;}
    void inc() {refCount++;}
    void dec() {if (refCount > 0) refCount--;}
    bool hasNoRefs() {return refCount == 0;}
    PtrType* get() {return ptr;}
    bool valid() {return validPtr;}
    
    void erase() {
        if (!validPtr) return;
        delete ptr;
        ptr = nullptr;
        validPtr = false;
    }
};

template<class PtrType> class IntelliPtr {
private:
    InternalPtr<PtrType>* internal;
public:
    IntelliPtr(IntelliPtr<PtrType>& last) {
        internal = last.internal;
        internal->inc();
    }
    
    IntelliPtr(PtrType* init) {
        internal = new InternalPtr<PtrType>(init);
        internal->set(init);
        internal->inc();
    }

    ~IntelliPtr() {
        internal->dec();
        if (internal->hasNoRefs()) delete internal;
    }

    PtrType* operator->() {return internal->get();}
    PtrType& operator*() {return *(internal->get());}

    PtrType* operator=(PtrType* now) {
        internal->set(now);
        return internal->get();
    }
    
    void erase() {internal->erase();}
    PtrType* get() {return internal->get();}
    bool valid() {return internal->valid();}
};

template<class DataType = int, bool undirected = true> class GElem {
public:
    typedef IntelliPtr<GElem<DataType, undirected>> Elem;
    typedef pair<Elem*, int> Content;
private:
    DataType data;
    unordered_map<Elem*, int> edges;
public:
    GElem(DataType init) {data = init;}
    
    ~GElem() {
        this->forEachLink([this](Elem current, int weight) -> void {
            this->abandon(current, true);
            current.erase();
        });
    }
    
    /*void forEachLink(function<void(Elem current, int weight)> fnc) {
        for (Content node : edges) {
            if (!(node.first)->valid() || node.first.get() == nullptr) {
                edges.erase(node.first);
                continue;
            }
            fnc(*(node.first), node.second);
        }
    }*/

    void forEachLink(function<bool(Elem current, int weight)> fnc) {
        for (Content node : edges) {
            if (!(node.first)->valid() || (node.first)->get() == nullptr) {
                edges.erase(node.first);
                continue;
            }
            if (fnc(*(node.first), node.second)) break;
        }
    }

    void link(Elem node, int weight = 1, bool linkOther = undirected) {
        edges.insert(Content(&node, weight));
        if (linkOther) node->link({this}, weight, false);
    }

    void abandon(Elem node, bool abandonOther = undirected) {
        edges.erase(&node);
        if (abandonOther) node->abandon({this}, false);
    }

    void orphan(bool abandonOthers = undirected) {
        this->forEachLink([this, abandonOthers](Elem current, int weight) -> bool {
            this->abandon(current, abandonOthers);
        });
    }

    void setData(DataType set) {data = set;}

    const DataType& getData() {return data;}
    const unordered_map<Elem*, int>& getEdges() {return edges;}

    bool isOrphan() {return edges.size() == 0;}
};

template<class DataType = int, bool undirected = true> class Graph {
public:
    typedef IntelliPtr<GElem<DataType, undirected>> Elem;
private:
    vector<Elem> roots;
    Elem current{nullptr};
    unsigned int size;

    Elem findIntern(DataType toBeFound, Elem parent, vector<Elem>& stack) {
        if (parent->getData() == toBeFound) return parent;
        else {
            Elem result{nullptr};
            parent->forEachLink([this, toBeFound, &result, &stack](Elem current, int weight) -> bool {
                auto place = std::find(stack.begin(), stack.end(), current);
                if (place == stack.end()) {
                    stack.push_back(current);
                    result = this->findIntern(toBeFound, current, stack);
                    if (result.get() != nullptr) return true;
                }
                return false;
            });
            return result;
        }
    }

    bool findPathIntern(Elem begin, Elem end, vector<Elem>& stack) {
        if (begin.get() == end.get()) return true;
        else {
            auto place = std::find(stack.begin(), stack.end(), begin);
            if (place == stack.end()) {
                stack.push_back(begin);
                bool result = false;
                begin->forEachLink([this, &end, &stack, &result](Elem current, int weight) -> bool {
                    result = this->findPathIntern(current, end, stack);
                    if (result) return true;
                    else return false;
                });
                return result;
            } else return false;
        }
    }

    vector<Elem> findRoots(Elem child) {
        vector<Elem> stack, rootsFound;
        for (Elem& root : roots) {
            stack.clear();
            if (this->findPathIntern(root, child, stack)) rootsFound.push_back(root);
        }
        return rootsFound;
    }

    bool joinIntern(Elem joined, int weight = 1) {
        if (current.get() == nullptr || current.get() == joined.get()) return false;
        if (joined.get() != nullptr) {
            current->link(joined, weight);
            if (roots.size() > 1) {
                vector<Elem> foundRoots = this->findRoots(current);
                if (foundRoots.size() > 1) for (int i = roots.size() - 1; i > 0; i--) {
                    auto place = std::find(foundRoots.begin(), foundRoots.end(), roots[i]);
                    if (place != foundRoots.end()) {
                        roots.erase(place);
                        foundRoots.erase(place);
                        if (foundRoots.size() == 1) break;
                    }
                }
            }
            return true;
        } else return false;
    }
    
    bool separateIntern(Elem separated) {
        if (current.get() == nullptr || current.get() == separated.get()) return false;
        if (separated != nullptr) {
            current->abandon(separated);
            vector<Elem>
                currentRoots = this->findRoots(current),
                separatedRoots = this->findRoots(separated);
            if (currentRoots.size() == 0) roots.push_back(current);
            if (separatedRoots.size() == 0) roots.push_back(separated);
            return true;
        } else return false;
    }
public:
    Graph() {}
    ~Graph() {
        for (Elem& root : roots) {
            root.erase();
        }
        roots.clear();
    }

    unsigned int getRootCount() {return roots.size();}

    Elem find(DataType toBeFound, bool setCurrent = true) {
        vector<Elem> stack;
        Elem result{nullptr};
        for (Elem& root : roots) {
            result = this->findIntern(toBeFound, root, stack);
            if (result.get() != nullptr) {
                if (setCurrent) current = result;
                return result;
            }
        }
        return {nullptr};
    }

    Elem& insert(DataType toBeInserted, bool setCurrent = true) {
        Elem elem{new GElem<DataType, undirected>(toBeInserted)};
        roots.push_back(elem);
        if (setCurrent) current = elem;
        return roots[roots.size() - 1];
    }

    bool erase(DataType toBeErased) {
        Elem found = this->find(toBeErased, false);
        if (!found) return false;
        else {
            found->orphan(true);
            delete found;
            return true;
        }
    }

    bool join(DataType toBeJoined, int weight = 1) {
        Elem joined = this->find(toBeJoined, false);
        return this->joinIntern(joined, weight);
    }

    bool joinTo(Elem joined, int weight = 1) {
        return this->joinIntern(joined, weight);
    }

    bool separate(DataType toBeSeparated, int weight = 1) {
        Elem separated = this->find(toBeSeparated, false);
        return this->separateIntern(separated, weight);
    }

    bool separateFrom(Elem separated, int weight = 1) {
        return this->separateIntern(separated, weight);
    }
};

#endif