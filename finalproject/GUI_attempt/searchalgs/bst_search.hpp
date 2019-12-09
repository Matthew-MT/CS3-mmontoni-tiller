#ifndef BST_SEARCH_HPP
#define BST_SEARCH_HPP

#include "bst.hpp"
#include <random>

namespace CS3::search {
    template<typename Type/*, typename Type::value_type*/> void bst_search(Type list) {
        BST<typename Type::value_type> tree;
        for (typename Type::value_type& val : list) tree.insertElement(val);
        tree.searchFor(std::rand());
    }
};

#endif