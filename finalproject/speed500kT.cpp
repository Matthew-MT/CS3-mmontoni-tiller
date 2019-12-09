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
    T vals500k(500000u, 0);
    std::function<long long(T testVal)> test500k;
    long double avg500k;

    std::string
        valLoc = "values500k.txt",
        resLoc = "results500k.txt";

    const std::size_t tests = 10u;

    vals500k = genRandRange<T>(500000u);

    std::ofstream values, results;
    values.open(valLoc, std::ofstream::out | std::ofstream::trunc);
    results.open(resLoc, std::ofstream::out | std::ofstream::trunc);
    if (!values.is_open() || !results.is_open()) throw "Could not open one or more files.\n";

    std::string output = "500k tested values:\n\n";
    for (int v : vals500k) output += to_string(v) + '\n';
    values << output;
    values.close();

    std::cout << "Running function: Generic unstable sort\n";

    test500k = timer<T>(CS3::sort::gen_sort<T>);
    avg500k = 0.f;
    for (std::size_t i = 0u; i < tests; i++) {
        std::cout << "| Test: " + std::to_string(i) + '/' + std::to_string(tests) + '\n';
        avg500k += (long double)test500k(vals500k);
    }
    avg500k /= tests;
    results << "Generic unstable sort, 500k values: " << avg500k << "\n\n";

    results.close();
    results.open(resLoc, std::ofstream::out | std::ofstream::app);

    std::cout << "Running function: Generic stable sort\n";

    test500k = timer<T>(CS3::sort::gen_stable_sort<T>);
    avg500k = 0.f;
    for (std::size_t i = 0u; i < tests; i++) {
        std::cout << "| Test: " + std::to_string(i) + '/' + std::to_string(tests) + '\n';
        avg500k += (long double)test500k(vals500k);
    }
    avg500k /= tests;
    results << "Generic stable sort, 500k values: " << avg500k << "\n\n";

    results.close();
    results.open(resLoc, std::ofstream::out | std::ofstream::app);

    std::cout << "Running function: Heap sort\n";

    test500k = timer<T>(CS3::sort::heapsort<T>);
    avg500k = 0.f;
    for (std::size_t i = 0u; i < tests; i++) {
        std::cout << "| Test: " + std::to_string(i) + '/' + std::to_string(tests) + '\n';
        avg500k += (long double)test500k(vals500k);
    }
    avg500k /= tests;
    results << "Heap sort, 500k values: " << avg500k << "\n\n";

    results.close();
    results.open(resLoc, std::ofstream::out | std::ofstream::app);

    std::cout << "Running function: Merge sort\n";

    test500k = timer<T>(CS3::sort::mergesort<T>);
    avg500k = 0.f;
    for (std::size_t i = 0u; i < tests; i++) {
        std::cout << "| Test: " + std::to_string(i) + '/' + std::to_string(tests) + '\n';
        avg500k += (long double)test500k(vals500k);
    }
    avg500k /= tests;
    results << "Merge sort, 500k values: " << avg500k << "\n\n";

    results.close();
    results.open(resLoc, std::ofstream::out | std::ofstream::app);

    std::cout << "Running function: Quick sort\n";

    test500k = timer<T>(CS3::sort::quicksort<T>);
    avg500k = 0.f;
    for (std::size_t i = 0u; i < tests; i++) {
        std::cout << "| Test: " + std::to_string(i) + '/' + std::to_string(tests) + '\n';
        avg500k += (long double)test500k(vals500k);
    }
    avg500k /= tests;
    results << "Quick sort, 500k values: " << avg500k << "\n\n";

    results.close();
    results.open(resLoc, std::ofstream::out | std::ofstream::app);

    std::cout << "Running function: Shell sort\n";

    test500k = timer<T>(CS3::sort::shellsort<T>);
    avg500k = 0.f;
    for (std::size_t i = 0u; i < tests; i++) {
        std::cout << "| Test: " + std::to_string(i) + '/' + std::to_string(tests) + '\n';
        avg500k += (long double)test500k(vals500k);
    }
    avg500k /= tests;
    results << "Shell sort, 500k values: " << avg500k << '\n';

    results.close();
    results.open(resLoc, std::ofstream::out | std::ofstream::app);

    std::cout << "Running function: Insertion sort\n";

    test500k = timer<T>(CS3::sort::insertionsort<T>);
    avg500k = 0.f;
    for (std::size_t i = 0u; i < tests; i++) {
        std::cout << "| Test: " + std::to_string(i) + '/' + std::to_string(tests) + '\n';
        avg500k += (long double)test500k(vals500k);
    }
    avg500k /= tests;
    results << "Insertion sort, 500k values: " << avg500k << "\n\n";

    results.close();
    results.open(resLoc, std::ofstream::out | std::ofstream::app);

    std::cout << "Running function: Selection sort\n";

    test500k = timer<T>(CS3::sort::selectionsort<T>);
    avg500k = 0.f;
    for (std::size_t i = 0u; i < tests; i++) {
        std::cout << "| Test: " + std::to_string(i) + '/' + std::to_string(tests) + '\n';
        avg500k += (long double)test500k(vals500k);
    }
    avg500k /= tests;
    results << "Selection sort, 500k values: " << avg500k << "\n\n";

    results.close();
    results.open(resLoc, std::ofstream::out | std::ofstream::app);

    std::cout << "Running function: Bubble sort\n";

    test500k = timer<T>(CS3::sort::bubblesort<T>);
    avg500k = 0.f;
    for (std::size_t i = 0u; i < tests; i++) {
        std::cout << "| Test: " + std::to_string(i) + '/' + std::to_string(tests) + '\n';
        avg500k += (long double)test500k(vals500k);
    }
    avg500k /= tests;
    results << "Bubble sort, 500k values: " << avg500k << "\n\n";

    results.close();
    std::cout << "Complete.\n";
    std::string end;
    std::cin >> end;
}