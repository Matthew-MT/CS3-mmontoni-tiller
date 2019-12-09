#include <unordered_map>
#include <functional>
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <thread>
#include <chrono>

// Sorting algorithms
#include "sortingalgs/bubblesort.hpp"
#include "sortingalgs/heapsort.hpp"
#include "sortingalgs/insertionsort.hpp"
#include "sortingalgs/mergesort.hpp"
#include "sortingalgs/quicksort.hpp"
#include "sortingalgs/selectionsort.hpp"
#include "sortingalgs/shellsort.hpp"
#include "sortingalgs/generic.hpp"

// Search algorithms
#include "searchalgs/bst_search.hpp"
#include "searchalgs/generic.hpp"

template<typename Type> Type genRandRange(std::size_t num) {
    Type returned(num, 0);
    for (std::size_t i = 0u; i < num; i++) returned[i] = std::rand();
    return std::move(returned);
}

template<typename Type>
std::function<long long(Type testVal)> timer(std::function<void(Type list)> fnc) {
    return [fnc](Type testVal) -> long long {
        std::clock_t time = std::clock();
        fnc(testVal);
        return std::clock() - time;
    };
}

int main() {
    using T = std::vector<int>;
    T
        vals100k(100000u, 0),
        vals500k(500000u, 0),
        vals1m(1000000u, 0);
    std::function<long long(T testVal)> tester;
    std::function<long long(BST<int>& testVal)> testerBST;
    long double avg100k, avg500k, avg1m;

    std::string
        valLoc = "searchvals.txt",
        resLoc = "searchres.txt";

    const std::size_t tests = 10u;

    vals100k = genRandRange<T>(100000u);
    vals500k = genRandRange<T>(500000u);
    vals1m = genRandRange<T>(1000000u);

    std::ofstream values, results;
    values.open(valLoc, std::ofstream::out | std::ofstream::trunc);
    results.open(resLoc, std::ofstream::out | std::ofstream::trunc);
    if (!values.is_open() || !results.is_open()) throw "Could not open one or more files.\n";

    std::string output = "100k tested values:\n\n";
    for (int v : vals100k) output += to_string(v) + '\n';
    values << output;
    values.close();


    BST<int> temp100k, temp500k, temp1m;
    CS3::search::makeBST(vals100k, temp100k);
    CS3::search::makeBST(vals500k, temp500k);
    CS3::search::makeBST(vals1m, temp1m);

    //std::cout << temp100k.getHeight();

    CS3::sort::quicksortRet<T>(vals100k);
    CS3::sort::quicksortRet<T>(vals500k);
    CS3::sort::quicksortRet<T>(vals1m);


    std::cout << "Running function: Binary search tree\n";

    testerBST = timer<BST<int>&>(CS3::search::bst_search<BST<int>&>);
    avg100k = 0.f, avg500k = 0.f, avg1m = 0.f;
    for (std::size_t i = 0u; i < tests; i++) {
        std::cout << "| Test: " + std::to_string(i) + '/' + std::to_string(tests) + '\n';
        //temp100k.searchFor(std::rand());
        avg100k += (long double)testerBST(temp100k);
        avg500k += (long double)testerBST(temp500k);
        avg1m += (long double)testerBST(temp1m);
    }
    avg100k /= tests;
    results
        << "Binary search tree, 100k values: " << avg100k
        << "\nBinary search tree, 500k values: " << avg500k
        << "\nBinary search tree, 1m values: " << avg1m << "\n\n";

    results.close();
    results.open(resLoc, std::ofstream::out | std::ofstream::app);


    std::cout << "Running function: Generic find\n";

    tester = timer<T>(CS3::search::gen_find<T>);
    avg100k = 0.f, avg500k = 0.f, avg1m = 0.f;
    for (std::size_t i = 0u; i < tests; i++) {
        std::cout << "| Test: " + std::to_string(i) + '/' + std::to_string(tests) + '\n';
        avg100k += (long double)tester(vals100k);
        avg500k += (long double)tester(vals500k);
        avg1m += (long double)tester(vals1m);
    }
    avg100k /= tests;
    results
        << "Generic find, 100k values: " << avg100k
        << "\nGeneric find, 500k values: " << avg500k
        << "\nGeneric find, 1m values: " << avg1m << "\n\n";

    results.close();
    results.open(resLoc, std::ofstream::out | std::ofstream::app);


    std::cout << "Running function: Generic binary search\n";

    tester = timer<T>(CS3::search::gen_bin_search<T>);
    avg100k = 0.f, avg500k = 0.f, avg1m = 0.f;
    for (std::size_t i = 0u; i < tests; i++) {
        std::cout << "| Test: " + std::to_string(i) + '/' + std::to_string(tests) + '\n';
        avg100k += (long double)tester(vals100k);
        avg500k += (long double)tester(vals500k);
        avg1m += (long double)tester(vals1m);
    }
    avg100k /= tests;
    results
        << "Generic binary search, 100k values: " << avg100k
        << "\nGeneric binary search, 500k values: " << avg500k
        << "\nGeneric binary search, 1m values: " << avg1m << "\n\n";

    results.close();
    std::cout << "Complete.\n";
    std::string end;
    std::cin >> end;
}