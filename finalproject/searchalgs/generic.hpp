#ifndef GENERICSEARCH_HPP
#define GENERICSEARCH_HPP

#include <algorithm>
#include <random>

namespace CS3::search {
    template<typename Type> void gen_find(Type list) {
        std::find(list.begin(), list.end(), std::rand());
    }
    template<typename Type> void gen_bin_search(Type list) {
        std::binary_search(list.begin(), list.end(), std::rand());
    }
};

#endif