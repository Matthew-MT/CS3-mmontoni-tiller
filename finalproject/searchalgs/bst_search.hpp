#ifndef BST_SEARCH_HPP
#define BST_SEARCH_HPP

#include "bst.hpp"
#include <random>
#include <vector>
#include <iostream>

namespace CS3::search {
    template<typename Type> void makeBST(std::vector<Type>& list, BST<Type>& tree) {
        for (Type& val : list) tree.insertElement(val);
    }
    template<typename Type/*, typename Type::value_type*/> void bst_search(Type list) {
        list.searchFor(std::rand());
    }
};

#endif