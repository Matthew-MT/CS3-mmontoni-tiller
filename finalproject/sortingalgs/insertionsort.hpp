#ifndef INSERTIONSORT_HPP
#define INSERTIONSORT_HPP

#include <algorithm>
#include <iostream>

namespace CS3::sort {
    template<typename Type> void insertionsortRet(Type& list) {
        for (std::size_t i = 1u; i < list.size(); i++) {
            for (std::size_t j = i; j > 0u && list[j] < list[j - 1u]; j--) std::swap(list[j], list[j - 1u]);
            //std::cout << "Finished pass " << i << '\n';
        }
        //return list;
    }
    template<typename Type> void insertionsort(Type list) {insertionsortRet(list);}
};

#endif