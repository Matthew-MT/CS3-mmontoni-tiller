#define SFML_STATIC

#include <SFML/Graphics.hpp>
#include "UI/scrollwindow.hpp"
#include "UI/constants.hpp"
#include "UI/concrete.hpp"
#include "UI/elements.hpp"
#include "UI/methods.hpp"
#include "UI/textbox.hpp"
#include "UI/button.hpp"
#include "UI/scroll.hpp"
#include "UI/scene.hpp"
#include "UI/pop.hpp"

#include "algorithm.hpp"

#if defined(_WIN32)
    #include <windows.h>
#endif

#include <functional>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <regex>

using namespace std;

int main(int argCount, char* args[]) {
    // Only show console if the argument "console" is used

#if defined(_WIN32)
    if (argCount > 1 && (string)args[1] == "console");
    else ShowWindow(FindWindowA("ConsoleWindowClass", NULL), false);
#endif

    // Main window & font declarations
    sf::RenderWindow window(sf::VideoMode(UI::winW, UI::winH), "Algorithm Compare", sf::Style::Default);
    sf::Font font;
    if (!font.loadFromFile("./fonts/cour.ttf")) throw "Couldn't find font";

    // Cursor alternatives declarations
    sf::Cursor normalCursor, textBoxCursor;
    normalCursor.loadFromSystem(sf::Cursor::Arrow);
    textBoxCursor.loadFromSystem(sf::Cursor::Text);

    // Runtime data
    sf::Clock clock;
    UI::Focus* focus = nullptr;
    UI::Hold* hold = nullptr;
    UI::Hover* hover = nullptr;
    UI::Scene* currentScene = nullptr;

    // Backgrounds
    sf::RectangleShape whiteSpace;
    whiteSpace.setSize({UI::winW, UI::winH});
    UI::PopBackground background({0, 0}, {UI::winW, UI::winH});
    background.setPadding(2.f);
    background.invert(true);
    
    CS3::MainHandler<vector<int>> tester;
    UI::Scene menuScene, progressScene, resultScene;
    size_t testNum = 1u, testSearchNum = 1u;
    ofstream res, vals;
    string which = "";

    UI::Button<>
        generateVals100k("Generate random numbers", font, {16.f, 16.f}),
        start100k("Begin test", font, {16.f, 48.f}),
        exitProgramFirst("Exit", font, {16.f, 80.f});
    //UI::TextBox_0 inputWhich(font, 16, {16.f, 112.f});
    sf::Text infoHeader("Select an option above", font, 20);
    infoHeader.setPosition({16.f, 180.f});
    infoHeader.setFillColor(UI::sBlack);
    
    vector<int>
        testVals100k(100000u, 0),
        testVals500k(500000u, 0),
        testVals1m(1000000u, 0),
        searchVals100k,
        searchVals500k,
        searchVals1m;

    generateVals100k.setHoverAction([](UI::Button<>* button) -> void {button->setFillColor(UI::sWhite, true);});
    generateVals100k.setUnHoverAction([](UI::Button<>* button) -> void {button->revert();});
    generateVals100k.setClickAction([](UI::Button<>* button) -> void {button->defaultClickEffect();});
    generateVals100k.setUnClickAction(
    [&font, &testVals100k, &testVals500k, &testVals1m, &searchVals100k, &searchVals500k, &searchVals1m, &infoHeader, &vals]
    (UI::Button<>* button) -> void {
        button->revert();
        searchVals100k = testVals100k = CS3::genRandRange<vector<int>>(100000u);
        CS3::sort::quicksortRet(searchVals100k);
        searchVals500k = testVals500k = CS3::genRandRange<vector<int>>(500000u);
        CS3::sort::quicksortRet(searchVals500k);
        searchVals1m = testVals1m = CS3::genRandRange<vector<int>>(1000000u);
        CS3::sort::quicksortRet(searchVals1m);
        vals.open("values.txt", ofstream::out | ofstream::trunc);
        if (!vals.is_open()) throw "Couldn't open value file\n";
        vals << "100,000 values:\n\n";
        for (int val : testVals100k) vals << to_string(val) + '\n';
        vals << "\n\n500,000 values:\n\n";
        for (int val : testVals500k) vals << to_string(val) + '\n';
        vals << "\n\n1,000,000 values:\n\n";
        for (int val : testVals1m) vals << to_string(val) + '\n';
        vals.close();
        //for (std::size_t i = 0u; i < 100000u; i++) testVals100k[i] = std::rand();
        infoHeader.setString("Values generated");
    });
    start100k.setHoverAction([](UI::Button<>* button) -> void {button->setFillColor(UI::sWhite, true);});
    start100k.setUnHoverAction([](UI::Button<>* button) -> void {button->revert();});
    start100k.setClickAction([](UI::Button<>* button) -> void {button->defaultClickEffect();});
    start100k.setUnClickAction([&tester, &testVals100k, &searchVals100k, &currentScene, &progressScene, &infoHeader/*, &which, &inputWhich*/]
    (UI::Button<>* button) -> void {
        button->revert();
        //which = inputWhich.getBuffer();
        tester.testInputs(testVals100k, 10u/*, which*/);
        tester.testSearch(searchVals100k, 10u/*, which*/);
        infoHeader.setString("Currently testing 100k values");
        cout << "Here\n";
        currentScene->switchAway();
        currentScene = &progressScene;
        currentScene->switchTo();
    });
    exitProgramFirst.setHoverAction([](UI::Button<>* button) -> void {button->setFillColor(UI::sWhite, true);});
    exitProgramFirst.setUnHoverAction([](UI::Button<>* button) -> void {button->revert();});
    exitProgramFirst.setClickAction([](UI::Button<>* button) -> void {button->defaultClickEffect();});
    exitProgramFirst.setUnClickAction([&window](UI::Button<>* button) -> void {
        button->revert();
        window.close();
    });
    /*inputWhich.setEnterAction([&start100k](UI::TextBox_0* textbox, string buffer) -> void {
        start100k.unClick();
    });*/
    menuScene.addElem(&whiteSpace);
    menuScene.addElem(&background);
    menuScene.addElem(&infoHeader);
    menuScene.addElem(&generateVals100k);
    menuScene.addElem(&start100k);
    menuScene.addElem(&exitProgramFirst);
    //menuScene.addElem(&inputWhich);


    sf::RectangleShape progressBar, progressInner;
    unordered_map<string, vector<pair<string, long double>>>
        times = {
            {"100k", {}},
            {"500k", {}},
            {"1m", {}}
        },
        timesSearch = {
            {"100k", {}},
            {"500k", {}},
            {"1m", {}}
        };
    progressBar.setPosition({16.f, (UI::winH / 2.f) - 16.f});
    progressBar.setSize({UI::winW - 32.f, 32.f});
    progressBar.setFillColor(sf::Color::Cyan);
    progressInner.setPosition(progressBar.getPosition());
    progressInner.setSize({0.f, 32.f});
    progressInner.setFillColor(sf::Color::Yellow);
    //progressScene.setSwitchToAction([testNum](UI::Scene* scene) -> void {infoHeader.setString("Test 1/3: 100k values");});
    progressScene.setSwitchToAction([&res](UI::Scene* scene) -> void {
        res.open("results.txt", ofstream::out | ofstream::trunc);
        if (!res.is_open()) throw "Couldn't open results file.\n";
    });
    progressScene.setTickAction(
    [&testNum, &testSearchNum, &tester, &times, &timesSearch, &res, &infoHeader,
    &testVals500k, &testVals1m, &searchVals500k, &searchVals1m,
    &currentScene, &resultScene, &progressBar, &progressInner, &which]
    (UI::Scene* scene) -> void {
        if (tester.complete()) {
            vector<pair<string, long double>> test = tester.getTimes();
            if (testNum == 1u) {
                string results = "";
                sort(test.begin(), test.end(), [](const auto& a, const auto& b) -> bool {return a.second < b.second;});
                for (auto& time : test) results += time.first + ": average of " + to_string(time.second) + " cycles\n";
                results += '\n';
                res << "Sorting results for 100k values:\n\n" << results;
                times["100k"] = move(test);
                tester.testInputs(testVals500k, 10u/*, which*/);
                infoHeader.setString("Currently testing 500k values...");
                testNum++;
            } else if (testNum == 2u) {
                string results = "";
                sort(test.begin(), test.end(), [](const auto& a, const auto& b) -> bool {return a.second < b.second;});
                for (auto& time : test) results += time.first + ": average of " + to_string(time.second) + " cycles\n";
                results += '\n';
                res << "\n\nSorting results for 500k values:\n\n" << results;
                times["500k"] = move(test);
                tester.testInputs(testVals1m, 10u/*, which*/);
                infoHeader.setString("Currently testing 1m values...");
                testNum++;
            } else if (testNum == 3u) {
                string results = "";
                sort(test.begin(), test.end(), [](const auto& a, const auto& b) -> bool {return a.second < b.second;});
                for (auto& time : test) results += time.first + ": average of " + to_string(time.second) + " cycles\n";
                results += '\n';
                res << "\n\nSorting results for 1m values:\n\n" << results;
                res.close();
                times["1m"] = move(test);
                if (testSearchNum == 3u && tester.completeSearch()) {
                    currentScene->switchAway();
                    currentScene = &resultScene;
                    currentScene->switchTo();
                }
            }
        } else {
            sf::Vector2f size = progressBar.getSize();
            std::size_t finished = tester.numFinished(), tests = tester.numTests();
            progressInner.setSize({((float)finished / (float)tests) * size.x, size.y});
            infoHeader.setString(
                string("Currently testing ") + string(testNum == 1u ? "100k" : (testNum == 2u ? "500k" : "1m")) + string(" values: ")
                + to_string(finished) + '/' + to_string(tests)
            );
        }
        if (tester.completeSearch()) {
            vector<pair<string, long double>> searchTest = tester.getTimesSearch();
            if (testSearchNum == 1u) {
                string results = "";
                sort(searchTest.begin(), searchTest.end(), [](const auto& a, const auto& b) -> bool {return a.second < b.second;});
                for (auto& time : searchTest) results += time.first + ": average of " + to_string(time.second) + " cycles\n";
                results += '\n';
                res << "Search results for 100k values:\n\n" << results;
                timesSearch["100k"] = move(searchTest);
                tester.testSearch(searchVals500k, 10u/*, which*/);
                testSearchNum++;
            } else if (testSearchNum == 2u) {
                string results = "";
                sort(searchTest.begin(), searchTest.end(), [](const auto& a, const auto& b) -> bool {return a.second < b.second;});
                for (auto& time : searchTest) results += time.first + ": average of " + to_string(time.second) + " cycles\n";
                results += '\n';
                res << "\n\nSorting results for 500k values:\n\n" << results;
                timesSearch["500k"] = move(searchTest);
                tester.testSearch(searchVals1m, 10u/*, which*/);
                infoHeader.setString("Currently testing 1m values...");
                testSearchNum++;
            } else if (testSearchNum == 3u) {
                string results = "";
                sort(searchTest.begin(), searchTest.end(), [](const auto& a, const auto& b) -> bool {return a.second < b.second;});
                for (auto& time : searchTest) results += time.first + ": average of " + to_string(time.second) + " cycles\n";
                results += '\n';
                res << "\n\nSorting results for 1m values:\n\n" << results;
                res.close();
                timesSearch["1m"] = move(searchTest);
            }
        }
    });
    progressScene.addElem(&whiteSpace);
    progressScene.addElem(&background);
    progressScene.addElem(&progressBar);
    progressScene.addElem(&progressInner);
    progressScene.addElem(&infoHeader);

    UI::ScrollWindow_0<> display100kTimes, display500kTimes, display1mTimes;
    UI::Button<>
        goToMenu("Main menu", font, {16.f, (UI::winH / 2.f) + 48.f}),
        exitProgramSecond("Exit", font, {16.f, (UI::winH / 2.f) + 48.f});
    display100kTimes.setPosition({16.f, 16.f});
    display100kTimes.setSize({96.f, (UI::winH / 2.f) - 16.f});
    display500kTimes.setPosition({112.f, 16.f});
    display500kTimes.setSize({96.f, (UI::winH / 2.f) - 16.f});
    display1mTimes.setPosition({208.f, 16.f});
    display1mTimes.setSize({96.f, (UI::winH / 2.f) - 16.f});
    unordered_map<string, vector<UI::Element>> displayLists = {
        {"100k", {}},
        {"500k", {}},
        {"1m", {}}
    };
    resultScene.setSwitchToAction(
    [&font, &times, &displayLists, &display100kTimes, &display500kTimes, &display1mTimes]
    (UI::Scene* scene) -> void {
        for (auto& set : times) for (auto& alg : set.second) displayLists[set.first].push_back({
            font, alg.first + ": " + to_string(alg.second / CLOCKS_PER_SEC) + " seconds"
        });
        display100kTimes.setData(&(displayLists["100k"]));
        display500kTimes.setData(&(displayLists["500k"]));
        display1mTimes.setData(&(displayLists["1m"]));
    });
    goToMenu.setHoverAction([](UI::Button<>* button) -> void {button->setFillColor(UI::sWhite, true);});
    goToMenu.setUnHoverAction([](UI::Button<>* button) -> void {button->revert();});
    goToMenu.setClickAction([](UI::Button<>* button) -> void {button->defaultClickEffect();});
    goToMenu.setUnClickAction([&currentScene, &menuScene](UI::Button<>* button) -> void {
        button->revert();
        currentScene->switchAway();
        currentScene = &menuScene;
        currentScene->switchTo();
    });
    exitProgramSecond.setHoverAction([](UI::Button<>* button) -> void {button->setFillColor(UI::sWhite, true);});
    exitProgramSecond.setUnHoverAction([](UI::Button<>* button) -> void {button->revert();});
    exitProgramSecond.setClickAction([](UI::Button<>* button) -> void {button->defaultClickEffect();});
    exitProgramSecond.setUnClickAction([&window](UI::Button<>* button) -> void {
        button->revert();
        window.close();
    });
    resultScene.addElem(&display100kTimes);
    resultScene.addElem(&display500kTimes);
    resultScene.addElem(&display1mTimes);
    resultScene.addElem(&goToMenu);
    resultScene.addElem(&exitProgramSecond);




    currentScene = &menuScene;
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            /**
             * Event loop, handles user actions and motions.
             */
            sf::Vector2i tempMousePos = sf::Mouse::getPosition(window);
            sf::Vector2f mousePos = {(float)tempMousePos.x, (float)tempMousePos.y};
            if (event.type == sf::Event::Closed) window.close();
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (focus != nullptr) {
                    if (!focus->isMouseOver(mousePos)) {
                        focus->endFocus();
                        focus = nullptr;
                    } else {
                        focus->startFocus(clock.getElapsedTime());
                        focus->click();
                    }
                }

                for (UI::Focus* val : currentScene->focusable) if (val->isMouseOver(mousePos)) {
                    focus = val;
                    break;
                }
                for (UI::Hold* val : currentScene->holdable) if (val->isMouseOver(mousePos)) {
                    hold = val;
                    break;
                }
                for (UI::Hover* val : currentScene->hoverable) if (val->isMouseOver(mousePos)) {
                    val->click();
                }

                if (hold != nullptr) hold->startHold(mousePos), hold->click();
            } else if (event.type == sf::Event::MouseButtonReleased) {
                if (hold != nullptr) {
                    hold->unClick();
                    hold->endHold();
                    hold = nullptr;
                }
                if (hover != nullptr) {
                    hover->unClick();
                    if (hover->isMouseOver(mousePos)) hover->hover();
                }
                if (focus != nullptr) focus->unClick();
            } else if (event.type == sf::Event::MouseMoved) {
                for (UI::Hover* val : currentScene->hoverable) {
                    if (val->isMouseOver(mousePos) && hover != val) {
                        if (hover != nullptr) hover->unHover();
                        hover = val;
                        break;
                    }
                }
                if (hold != nullptr && sf::Mouse::isButtonPressed(sf::Mouse::Left)) hold->updateHold(mousePos);
                if (hover != nullptr) {
                    if (hover->isMouseOver(mousePos)) hover->hover();
                    else {
                        hover->unHover();
                        hover = nullptr;
                    }
                }
            } else if (event.type == sf::Event::KeyReleased) {
                if (focus != nullptr) focus->registerKeystroke(
                    event.key.code,
                    sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)
                );
            }
        }
        sf::Time time = clock.getElapsedTime();
        if (focus != nullptr) focus->focusTick(time);

        /**
         * Draw the current scene.
         */
        this_thread::sleep_for(chrono::milliseconds(32));
        window.clear();
        if (currentScene != nullptr) {
            window.draw(*currentScene);
            currentScene->tick();
        }
        window.display();
    }
    
#if defined(_WIN32)
    FreeConsole();
#endif

    return 0;
}