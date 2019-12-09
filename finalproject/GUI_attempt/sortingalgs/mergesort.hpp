#ifndef MERGESORT_HPP
#define MERGESORT_HPP

#include <algorithm>

namespace CS3::sort {
    template<typename Type> Type& mergeSortSpecial(Type& list) {
        if (list.size() == 1u) return list;
        else {
            std::size_t mid = list.size() / 2u;
            Type l0(mid), l1(list.size() - mid);
            for (std::size_t i = 0u; i < list.size(); i++) {
                if (i < mid) l0[i] = list[i];
                else l1[i - mid] = list[i];
            }
            l0 = mergeSortSpecial(l0), l1 = mergeSortSpecial(l1);
            for (std::size_t i = 0u, i0 = 0u, i1 = 0u; i < list.size(); i++) list[i] = (l0[i0] < l1[i1] ? l0[i0++] : l1[i1++]);
            return list;
        }
    }
    template<typename Type> void mergesortRet(Type& list) {mergeSortSpecial(list);}
    template<typename Type> void mergesort(Type list) {mergesortRet(list);}
};

#endif