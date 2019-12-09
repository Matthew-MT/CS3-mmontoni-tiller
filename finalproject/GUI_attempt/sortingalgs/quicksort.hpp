#ifndef QUICKSORT_HPP
#define QUICKSORT_HPP

#include <algorithm>

namespace CS3::sort {
    template<typename Type> std::size_t partitionSpecial(Type& list, std::size_t left, std::size_t right, std::size_t pivot) {
        while (right > left) {
            while (list[left] < list[pivot] && left < list.size() - 1) left++;
            while (right >= left && right > 0u && list[right] >= list[pivot]) right--;
            if (right > left) std::swap(list[left], list[right]);
        }
        return left;
    }
    template<typename Type> void quicksortSpecial(Type& list, std::size_t first, std::size_t last) {
        if (first < last) {
            std::size_t pivot = (first + last) / 2u;

            std::swap(list[pivot], list[last]);
            std::size_t val = partitionSpecial<Type>(list, first, last - 1u, last);
            std::swap(list[val], list[last]);
            if (first < val) quicksortSpecial<Type>(list, first, val - 1u);
            if (val < last) quicksortSpecial<Type>(list, val + 1, last);
        }
        //return list;
    }
    template<typename Type> void quicksortRet(Type& list) {quicksortSpecial(list, 0u, list.size() - 1);}
    template<typename Type> void quicksort(Type list) {quicksortRet(list);}
};

#endif