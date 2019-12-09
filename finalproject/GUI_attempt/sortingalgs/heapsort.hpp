#ifndef HEAPSORT_HPP
#define HEAPSORT_HPP

#include <algorithm>
#include <queue>

namespace CS3::sort {
    template<typename Type> void heapsortRet(Type& list) {
        std::priority_queue<typename Type::value_type> minHeap;
        for (std::size_t i = 0u; i < list.size(); i++) minHeap.push(list[i]);
        Type sorted(list.size());
        for (std::size_t i = 0u; i < list.size(); i++) {
            sorted[i] = minHeap.top();
            minHeap.pop();
        }
        //return list;
    }
    template<typename Type> void heapsort(Type list) {heapsortRet(list);}
};

#endif