#ifndef SELECTIONSORT_HPP
#define SELECTIONSORT_HPP

#include <algorithm>
#include <iostream>
#include <string>

namespace CS3::sort {
    template<typename Type> void selectionsortRet(Type& list) {
        for (std::size_t i = 0u; i < list.size() - 1; i++) {
            std::size_t min = i;
            for (std::size_t j = i + 1; j < list.size(); j++) if (list[j] < list[min]) min = j;
            //std::cout << "Min: " << min << '\n';
            if (list[min] != list[i]) std::swap(list[min], list[i]);
        }
        //return list;
    }
    template<typename Type> void selectionsort(Type list) {selectionsortRet(list);}
};

#endif