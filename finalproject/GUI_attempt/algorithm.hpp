#ifndef ALGORITHM_MAIN_HPP
#define ALGORITHM_MAIN_HPP

#include <SFML/System.hpp>
#include <unordered_map>
#include <functional>
#include <iostream>
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

namespace CS3 {
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

    template<typename SortedType> class MainHandler {
    private:
        std::unordered_map<std::string, std::pair<std::function<long long(SortedType testVal)>, long double>> sortingTypes = {
            //{"bubble", {timer<SortedType>(sort::bubblesort<SortedType>), 0.f}},
            //{"selection", {timer<SortedType>(sort::selectionsort<SortedType>), 0.f}},
            //{"insertion", {timer<SortedType>(sort::insertionsort<SortedType>), 0.f}},
            {"shell", {timer<SortedType>(sort::shellsort<SortedType>), 0.f}},
            {"quick", {timer<SortedType>(sort::quicksort<SortedType>), 0.f}},
            {"merge", {timer<SortedType>(sort::mergesort<SortedType>), 0.f}},
            {"heap", {timer<SortedType>(sort::heapsort<SortedType>), 0.f}},
            {"sort", {timer<SortedType>(sort::gen_sort<SortedType>), 0.f}},
            {"stable_sort", {timer<SortedType>(sort::gen_stable_sort<SortedType>), 0.f}}
        };

        std::unordered_map<std::string, std::pair<std::function<long long(SortedType testVal)>, long double>> searchTypes = {
            {"linear", {timer<SortedType>(search::gen_find<SortedType>), 0.f}},
            {"binary", {timer<SortedType>(search::gen_bin_search<SortedType>), 0.f}},
            {"bst", {timer<SortedType>(search::bst_search<SortedType>), 0.f}}
        };

        sf::Thread* handlerSort = nullptr, * handlerSearch = nullptr;
        bool inProgressSort = false, inProgressSearch = false;
        class {
        public: std::size_t cur = 0u, num = 0u;
        } trackerSort, trackerSearch;
    public:
        MainHandler() {}

        void testInputs(SortedType& inputs, std::size_t times/*, std::string which*/) {
            if (inProgressSort) throw "Cannot interrupt execution of previous test.\n";
            //if (sortingTypes.count(which) == 0u) throw "Couldn't find function.\n";
            if (handlerSort != nullptr) delete handlerSort;
            inProgressSort = true;
            trackerSort.cur = 0u;
            trackerSort.num = sortingTypes.size() * times;
            handlerSort = new sf::Thread([this, &inputs, times/*, which*/]() -> void {
                try {try {
                    std::cout << "Beginning test...\n";
                    for (auto& alg : this->sortingTypes) {
                        //auto& alg = this->sortingTypes[which];
                        long double avg = 0.f;
                        std::cout << "Executing function: " + alg.first + '\n';
                        for (std::size_t i = 0u; i < times; i++) {
                            avg += (long double)alg.second.first(inputs);
                            this->trackerSort.cur++;
                            //this_thread::sleep_for(chrono::milliseconds(32));
                        }
                        avg /= (long double)times;
                        alg.second.second = avg;
                    }
                    this->inProgressSort = false;
                    std::cout << "Test end.\n";
                } catch (const char* err) {std::cout << err;} catch (const std::exception& err) {std::cout << err.what();} catch (...) {}
                } catch (...) {std::cout << "Couldn't catch something.\n";}
            });
            handlerSort->launch();
        }

        void testSearch(SortedType& inputs, std::size_t times/*, std::string which*/) {
            if (inProgressSearch) throw "Cannot interrupt execution of previous test.\n";
            //if (searchTypes.count(which) == 0u) throw "Couldn't find function.\n";
            if (handlerSearch != nullptr) delete handlerSearch;
            inProgressSearch = true;
            trackerSearch.cur = 0u;
            trackerSearch.num = 3u * times;
            handlerSearch = new sf::Thread([this, &inputs, times/*, which*/]() -> void {
                std::cout << "Beginning test...\n";
                for (auto& alg : this->searchTypes) {
                    //auto& alg = this->searchTypes[which];
                    long double avg = 0.f;
                    std::cout << "Executing function: " + alg.first + '\n';
                    for (std::size_t i = 0u; i < times; i++) {
                        avg += (long double)alg.second.first(inputs);
                        this->trackerSearch.cur++;
                        //this_thread::sleep_for(chrono::milliseconds(32));
                    }
                    avg /= (long double)times;
                    alg.second.second = avg;
                }
                this->inProgressSearch = false;
                cout << "Test end.\n";
            });
            handlerSearch->launch();
        }

        std::size_t numFinished() {return trackerSort.cur;}
        std::size_t numTests() {return trackerSort.num;}
        std::size_t numFinishedSearch() {return trackerSearch.cur;}
        std::size_t numTestsSearch() {return trackerSearch.num;}

        std::vector<std::pair<string, long double>> getTimes() {
            std::vector<std::pair<string, long double>> times;
            for (auto& alg : sortingTypes) times.push_back({alg.first, alg.second.second});
            return std::move(times);
        }

        std::vector<std::pair<string, long double>> getTimesSearch() {
            std::vector<std::pair<string, long double>> times;
            for (auto& alg : searchTypes) times.push_back({alg.first, alg.second.second});
            return std::move(times);
        }

        bool complete() {return this->numFinished() == this->numTests();}
        bool completeSearch() {return this->numFinishedSearch() == this->numTestsSearch();}
    };
};

#endif