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
    T vals1m(1000000u, 0);
    std::function<long long(T testVal)> test1m;
    long double avg1m;

    std::string
        valLoc = "values1m.txt",
        resLoc = "results1m.txt";

    const std::size_t tests = 10u;

    vals1m = genRandRange<T>(1000000u);

    std::ofstream values, results;
    values.open(valLoc, std::ofstream::out | std::ofstream::trunc);
    results.open(resLoc, std::ofstream::out | std::ofstream::trunc);
    if (!values.is_open() || !results.is_open()) throw "Could not open one or more files.\n";

    std::string output = "1m tested values:\n\n";
    for (int v : vals1m) output += to_string(v) + '\n';
    values << output;
    values.close();


    std::cout << "Running function: Generic unstable sort\n";

    test1m = timer<T>(CS3::sort::gen_sort<T>);
    avg1m = 0.f;
    for (std::size_t i = 0u; i < tests; i++) {
        std::cout << "| Test: " + std::to_string(i) + '/' + std::to_string(tests) + '\n';
        avg1m += (long double)test1m(vals1m);
    }
    avg1m /= tests;
    results << "Generic unstable sort, 1m values: " << avg1m << "\n\n";

    results.close();
    results.open(resLoc, std::ofstream::out | std::ofstream::app);

    std::cout << "Running function: Generic stable sort\n";

    test1m = timer<T>(CS3::sort::gen_stable_sort<T>);
    avg1m = 0.f;
    for (std::size_t i = 0u; i < tests; i++) {
        std::cout << "| Test: " + std::to_string(i) + '/' + std::to_string(tests) + '\n';
        avg1m += (long double)test1m(vals1m);
    }
    avg1m /= tests;
    results << "Generic stable sort, 1m values: " << avg1m << "\n\n";

    results.close();
    results.open(resLoc, std::ofstream::out | std::ofstream::app);

    std::cout << "Running function: Heap sort\n";

    test1m = timer<T>(CS3::sort::heapsort<T>);
    avg1m = 0.f;
    for (std::size_t i = 0u; i < tests; i++) {
        std::cout << "| Test: " + std::to_string(i) + '/' + std::to_string(tests) + '\n';
        avg1m += (long double)test1m(vals1m);
    }
    avg1m /= tests;
    results << "Heap sort, 1m values: " << avg1m << "\n\n";

    results.close();
    results.open(resLoc, std::ofstream::out | std::ofstream::app);

    std::cout << "Running function: Merge sort\n";

    test1m = timer<T>(CS3::sort::mergesort<T>);
    avg1m = 0.f;
    for (std::size_t i = 0u; i < tests; i++) {
        std::cout << "| Test: " + std::to_string(i) + '/' + std::to_string(tests) + '\n';
        avg1m += (long double)test1m(vals1m);
    }
    avg1m /= tests;
    results << "Merge sort, 1m values: " << avg1m << "\n\n";

    results.close();
    results.open(resLoc, std::ofstream::out | std::ofstream::app);

    std::cout << "Running function: Quick sort\n";

    test1m = timer<T>(CS3::sort::quicksort<T>);
    avg1m = 0.f;
    for (std::size_t i = 0u; i < tests; i++) {
        std::cout << "| Test: " + std::to_string(i) + '/' + std::to_string(tests) + '\n';
        avg1m += (long double)test1m(vals1m);
    }
    avg1m /= tests;
    results << "Quick sort, 1m values: " << avg1m << "\n\n";

    results.close();
    results.open(resLoc, std::ofstream::out | std::ofstream::app);

    std::cout << "Running function: Shell sort\n";

    test1m = timer<T>(CS3::sort::shellsort<T>);
    avg1m = 0.f;
    for (std::size_t i = 0u; i < tests; i++) {
        std::cout << "| Test: " + std::to_string(i) + '/' + std::to_string(tests) + '\n';
        avg1m += (long double)test1m(vals1m);
    }
    avg1m /= tests;
    results << "Shell sort, 1m values: " << avg1m << '\n';

    results.close();
    results.open(resLoc, std::ofstream::out | std::ofstream::app);

    std::cout << "Running function: Insertion sort\n";

    test1m = timer<T>(CS3::sort::insertionsort<T>);
    avg1m = 0.f;
    for (std::size_t i = 0u; i < tests; i++) {
        std::cout << "| Test: " + std::to_string(i) + '/' + std::to_string(tests) + '\n';
        avg1m += (long double)test1m(vals1m);
    }
    avg1m /= tests;
    results << "Insertion sort, 1m values: " << avg1m << "\n\n";

    results.close();
    results.open(resLoc, std::ofstream::out | std::ofstream::app);

    std::cout << "Running function: Selection sort\n";

    test1m = timer<T>(CS3::sort::selectionsort<T>);
    avg1m = 0.f;
    for (std::size_t i = 0u; i < tests; i++) {
        std::cout << "| Test: " + std::to_string(i) + '/' + std::to_string(tests) + '\n';
        avg1m += (long double)test1m(vals1m);
    }
    avg1m /= tests;
    results << "Selection sort, 1m values: " << avg1m << "\n\n";

    results.close();
    results.open(resLoc, std::ofstream::out | std::ofstream::app);

    std::cout << "Running function: Bubble sort\n";

    test1m = timer<T>(CS3::sort::bubblesort<T>);
    avg1m = 0.f;
    for (std::size_t i = 0u; i < tests; i++) {
        std::cout << "| Test: " + std::to_string(i) + '/' + std::to_string(tests) + '\n';
        avg1m += (long double)test1m(vals1m);
    }
    avg1m /= tests;
    results << "Bubble sort, 1m values: " << avg1m << "\n\n";
    results.close();
    std::cout << "Complete.\n";
    std::string end;
    std::cin >> end;
}