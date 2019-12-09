#ifndef BUBBLESORT_HPP
#define BUBBLESORT_HPP

#include <algorithm>

namespace CS3::sort {
    template<typename Type> void bubblesortRet(Type& list) {
        bool sorted = true;
        for (std::size_t i = 0u; i < list.size() - 1u; i++) {
            sorted = true;
            for (std::size_t j = 1u; j < list.size() - i; j++) {
                if (list[j - 1u] > list[j]) {
                    std::swap(list[j - 1u], list[j]);
                    sorted = false;
                }
            }
            if (sorted) break;
        }
        //return list;
    }
    template<typename Type> void bubblesort(Type list) {bubblesortRet(list);}
};

#endif