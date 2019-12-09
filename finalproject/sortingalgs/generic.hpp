#ifndef GENERICSORT_HPP
#define GENERICSORT_HPP

#include <algorithm>

namespace CS3::sort {
    template<typename Type> void gen_sort(Type list) {std::sort(list.begin(), list.end());}
    template<typename Type> void gen_stable_sort(Type list) {std::stable_sort(list.begin(), list.end());}
};

#endif