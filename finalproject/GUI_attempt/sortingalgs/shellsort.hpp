#ifndef SHELLSORT_HPP
#define SHELLSORT_HPP

#include <algorithm>

namespace CS3::sort {
    template<typename Type> void insSortSpecial(Type& list, std::size_t s, std::size_t inc) {
        for (std::size_t i = s + inc; i < list.size(); i += inc) {
            for (std::size_t j = i; j >= inc && list[j] < list[j - inc]; j -= inc) std::swap(list[j], list[j - inc]);
        }
        //return list;
    }
    template<typename Type> void shellsortRet(Type& list) {
        for (std::size_t i = (std::size_t)(list.size() / 2u); i > 2u; i /= 2) {
            for (std::size_t j = 0u; j < i; j++) insSortSpecial<Type>(list, j, i);
        }
        //return list;
    }
    template<typename Type> void shellsort(Type list) {shellsortRet(list);}
};

#endif