/**
 * Compile with:
 * g++ -o test0.exe test0.cpp -Isfml/include -Lsfml/lib "-Wl,--start-group" -lfreetype -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lsfml-main -lgdi32 -lopengl32 -lwinmm "-Wl,--end-group"
 * 
 * Author: Matthew MT
 * Date completed: 10/25/19
 * Additional information:
 * All code relevant to the assignment is located in this file and the file bst.hpp.
 * Since this file also contains the necessary code to make the interface work properly,
 * code that is directly relevant is commented thoroughly.
 * 
 * Note that the file "bst.exe" can be run on Windows.
 * It should work for versions of Windows other than Windows 10,
 * but it has only been tested on Windows 10.
 * 
 * Also note that my VSCode tells me there are a couple of errors, but it compiles and runs just fine.
 */

#define SFML_STATIC

#include <SFML/Graphics.hpp>
#include "scrollwindow.hpp"
#include "constants.hpp"
#include "concrete.hpp"
#include "elements.hpp"
#include "methods.hpp"
#include "textbox.hpp"
#include <functional>
#include "button.hpp"
#include "scroll.hpp"
#include <windows.h>
#include "scene.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "pop.hpp"
#include "bst.hpp"
#include <string>
#include <regex>

using namespace std;

/**
 * Note: utility function to convert BSTElems to something that can be drawn on screen.
 */
vector<Element>& convertToVectOfElems(sf::Font& font, const vector<BSTElem<string>*>& vect, vector<Element>& output) {
    output.clear();
    for (BSTElem<>* val : vect) output.push_back(
        {font, val->getData() + ": " + to_string(val->getCount()) + " times"}
    );
    return output;
}

int main(int argCount, char* args[]) {
    // Only show console if the argument "console" is used
    if (argCount > 1 && (string)args[1] == "console");
    else ShowWindow(FindWindowA("ConsoleWindowClass", NULL), false);

    // Main window & font declarations
    sf::RenderWindow window(sf::VideoMode(winW, winH), "Binary Search Tree", sf::Style::Default);
    sf::Font font;
    if (!font.loadFromFile("./fonts/cour.ttf")) throw "Couldn't find font";

    // Cursor alternatives declarations
    sf::Cursor normalCursor, textBoxCursor;
    normalCursor.loadFromSystem(sf::Cursor::Arrow);
    textBoxCursor.loadFromSystem(sf::Cursor::Text);

    // Runtime data
    sf::Clock clock;
    Focus* focus = nullptr;
    Hold* hold = nullptr;
    Hover* hover = nullptr;
    Scene* currentScene = nullptr;

    // Backgrounds
    sf::RectangleShape whiteSpace;
    whiteSpace.setSize({winW, winH});
    PopBackground background({0, 0}, {winW, winH});
    background.setPadding(2.f);
    background.invert(true);

    /**
     * Declaration of BST.
     * Default type of "string" used for assignment.
     */
    BST<>* bst = new BST<>();

    /**
     * The file to read from in order to retrieve user input.
     */
    ifstream file;

    /**
     * Scene 1, contains input field for the file stream specified above.
     * Declarations:
     * Header, Input, Button
     */
    Scene enterFile, displayContent;
    sf::Text headerFileEntry("Please input file name below:", font, 28);
    TextBox_0 fileInput(font, 16, {(winW / 2.f) - 96.f, winH / 2.f}, {2.f, 2.f}, {128.f, -1.f});
    Button<> goToContent("Submit", font, {(winW / 2.f) + 48.f, winH / 2.f});

    // Add the backgrounds to the scene
    enterFile.drawable.push_back(&whiteSpace);
    enterFile.drawable.push_back(&background);

    // Add all the elements to their respective handlers
    // FUTURE: Make generic function to enable cleaner code
    sf::FloatRect headerBounds = headerFileEntry.getLocalBounds();
    headerFileEntry.setPosition(centerStrInBounds({headerBounds.width, headerBounds.height}, {winW, winH * 0.8}));
    headerFileEntry.setFillColor(sBlack);
    enterFile.drawable.push_back(&headerFileEntry);
    enterFile.focusable.push_back(&fileInput);
    enterFile.hoverable.push_back(&fileInput);
    enterFile.drawable.push_back(&fileInput);
    enterFile.drawable.push_back(&goToContent);
    enterFile.hoverable.push_back(&goToContent);

    /**
     * Add handlers for the text input, pressing "enter" will automatically load the next scene.
     */
    fileInput.setHoverAction([&window, &textBoxCursor](TextBox_0* textbox) -> void {window.setMouseCursor(textBoxCursor);});
    fileInput.setUnHoverAction([&window, &normalCursor](TextBox_0* textbox) -> void {window.setMouseCursor(normalCursor);});
    fileInput.setEnterAction([&file, &headerFileEntry, &currentScene, &displayContent](TextBox_0* textbox, string buffer) -> void {
        try {
            /**
             * Attempt to open the specified file.
             * If the file doesn't exist, notify the user.
             */
            file.open(buffer);
            if (!file.is_open()) {
                headerFileEntry.setString("Couldn't open file. Try again:");
                textbox->setBuffer();
            } else {
                currentScene->switchAway();
                currentScene = &displayContent;
                currentScene->switchTo();
            }
        } catch (string err) {
            headerFileEntry.setString("Error with file reading! Please try again:");
            textbox->setBuffer();
        };
    });
    goToContent.setHoverAction([](Button<>* button) -> void {button->setFillColor(sWhite, true);});
    goToContent.setUnHoverAction([](Button<>* button) -> void {button->revert();});
    goToContent.setClickAction([](Button<>* button) -> void {button->defaultClickEffect();});
    goToContent.setUnClickAction([&file, &headerFileEntry, &currentScene, &displayContent, &fileInput](Button<>* button) -> void {
        button->revert();
        try {
            /**
             * Clicking the button does the same thing as pressing "enter" in the text input.
             * See above.
             */
            file.open(fileInput.getBuffer());
            if (!file.is_open()) {
                headerFileEntry.setString("Couldn't open file. Try again:");
                fileInput.setBuffer();
            } else {
                currentScene->switchAway();
                currentScene = &displayContent;
                currentScene->switchTo();
            }
        } catch (string err) {
            headerFileEntry.setString("Error with file reading! Please try again:");
            fileInput.setBuffer();
        };
    });


    /**
     * Declarations for the data scene.
     * Note that the scene itself was declared above,
     * these lines only do the job of adding elements.
     */
    sf::Vector2f
        DC_Pad = {16.f, 16.f}, // Make it easier to change values
        DC_SWSize = {164.f, 192.f},
        DC_TBSize = {64.f, -1.f},
        DC_DistOfOpts = {32.f, 24.f};
    string defaultOrderDisplay = "Currently displayed as: ", order = "inorder"; // Remind the user which order the words are in
    vector<Element> scrollDisplay;
    sf::Text
        headerContent(defaultOrderDisplay + order, font, 24),
        responseHeader("Select an option above", font, 18);
    ScrollWindow_0<Element> fileDataDisplay(nullptr, DC_Pad, DC_SWSize); // Displays the inputted and parsed text
    Button<> // All the buttons!
        searchWord("Search word", font,                                                     // Search for a word
            {DC_Pad.x + DC_SWSize.x + DC_TBSize.x + (DC_DistOfOpts.x * 2.f), DC_Pad.y}),
        printTopTen("Print top ten into file", font,                                        // Print the top ten words to the specified file
            {DC_Pad.x + DC_SWSize.x + DC_TBSize.x + (DC_DistOfOpts.x * 2.f), DC_Pad.y + DC_DistOfOpts.y}),
        deleteWord("Delete word", font,                                                     // Delete a word
            {DC_Pad.x + DC_SWSize.x + DC_TBSize.x + (DC_DistOfOpts.x * 2.f), DC_Pad.y + (DC_DistOfOpts.y * 2.f)}),
        displayPreOrder("Display preorder traversal", font,                                 // Change the display order to preorder
            {DC_Pad.x + DC_SWSize.x + DC_DistOfOpts.x, DC_Pad.y + (DC_DistOfOpts.y * 3.f)}),
        displayInOrder("Display inorder traversal", font,                                   // Change the display order to inorder
            {DC_Pad.x + DC_SWSize.x + DC_DistOfOpts.x, DC_Pad.y + (DC_DistOfOpts.y * 4.f)}),
        displayPostOrder("Display postorder traversal", font,                               // Change the display order to postorder
            {DC_Pad.x + DC_SWSize.x + DC_DistOfOpts.x, DC_Pad.y + (DC_DistOfOpts.y * 5.f)}),
        exitProgram("Exit", font,                                                           // Exit the program
            {DC_Pad.x + DC_SWSize.x + DC_DistOfOpts.x, DC_Pad.y + (DC_DistOfOpts.y * 6.f)});
    TextBox_0
        wordSearch(font, textSize, {DC_Pad.x + DC_SWSize.x + DC_DistOfOpts.x, DC_Pad.y}), // Input word to search
        topTen(font, textSize, {DC_Pad.x + DC_SWSize.x + DC_DistOfOpts.x, DC_Pad.y + DC_DistOfOpts.y}), // Input file to write to
        wordDelete(font, textSize, {DC_Pad.x + DC_SWSize.x + DC_DistOfOpts.x, DC_Pad.y + (DC_DistOfOpts.y * 2.f)}); // Input word to delete
    
    /**
     * Add the base elements to the scene.
     */
    displayContent.drawable.push_back(&whiteSpace);
    displayContent.drawable.push_back(&background);
    
    displayContent.drawable.push_back(&headerContent);
    displayContent.drawable.push_back(&responseHeader);
    displayContent.drawable.push_back(&fileDataDisplay);
    displayContent.hoverable.push_back(&fileDataDisplay);
    displayContent.holdable.push_back(&fileDataDisplay);

    /**
     * Set the postion of the elements that do not have an initialized position.
     */
    headerContent.setPosition({16.f, 224.f});
    headerContent.setFillColor(sBlack);
    responseHeader.setPosition(
        {centerStrInBounds({responseHeader.getLocalBounds().width, responseHeader.getLocalBounds().height}, {winW, 0.f}).x, 256.f}
    );
    responseHeader.setFillColor(sBlack);
    fileDataDisplay.setHoverAction([](Scroll* scrollbar) -> void {scrollbar->setFillColor(sWhite, true);});
    fileDataDisplay.setUnHoverAction([](Scroll* scrollbar) -> void {scrollbar->revert();});
    fileDataDisplay.setClickAction([](Scroll* scrollbar) -> void {scrollbar->invert(true);});
    fileDataDisplay.setUnClickAction([](Scroll* scrollbar) -> void {scrollbar->revert();});

    /**
     * When this scene is switched to, parse the elements given from the file and insert into the tree.
     */
    displayContent.setSwitchToAction([&file, &bst, &fileDataDisplay, &scrollDisplay, &font](Scene* scene) -> void {
        smatch returned;
        regex parser("\\W*(\\w+)\\W*");
        while (!file.eof()) { // Iterate until the end of the file
            string temp, word, lowered;
            file >> temp;
            regex_match(temp, returned, parser);
            word = returned[1];
            for (int i = 0; i < word.size(); i++) lowered += tolower(word[i]);
            if (word.size() > 0) bst->insertElement(lowered);
        }
        file.close(); // Once done, close the file
        vector<BSTElem<>*> traversal = bst->traverse(BST<>::inorder); // Initialize the display to inorder traversal
        convertToVectOfElems(font, traversal, scrollDisplay);
        fileDataDisplay.setData(&scrollDisplay);
    });

    /**
     * Add all the rest of the elements to the scene.
     */
    displayContent.focusable.push_back(&wordSearch);
    displayContent.focusable.push_back(&topTen);
    displayContent.focusable.push_back(&wordDelete);
    displayContent.hoverable.push_back(&wordSearch);
    displayContent.hoverable.push_back(&topTen);
    displayContent.hoverable.push_back(&wordDelete);
    displayContent.drawable.push_back(&wordSearch);
    displayContent.drawable.push_back(&topTen);
    displayContent.drawable.push_back(&wordDelete);

    displayContent.drawable.push_back(&searchWord);
    displayContent.drawable.push_back(&printTopTen);
    displayContent.drawable.push_back(&deleteWord);
    displayContent.drawable.push_back(&displayPreOrder);
    displayContent.drawable.push_back(&displayInOrder);
    displayContent.drawable.push_back(&displayPostOrder);
    displayContent.drawable.push_back(&exitProgram);

    displayContent.hoverable.push_back(&searchWord);
    displayContent.hoverable.push_back(&printTopTen);
    displayContent.hoverable.push_back(&deleteWord);
    displayContent.hoverable.push_back(&displayPreOrder);
    displayContent.hoverable.push_back(&displayInOrder);
    displayContent.hoverable.push_back(&displayPostOrder);
    displayContent.hoverable.push_back(&exitProgram);

    /**
     * Set the actions for when the user hovers over buttons, input boxes, etc.
     */
    wordSearch.setHoverAction([&window, &textBoxCursor](TextBox_0* textbox) -> void {window.setMouseCursor(textBoxCursor);});
    wordSearch.setUnHoverAction([&window, &normalCursor](TextBox_0* textbox) -> void {window.setMouseCursor(normalCursor);});
    wordSearch.setEnterAction([&bst, &responseHeader](TextBox_0* textbox, string buffer) -> void {
        /**
         * Search for the specified word.
         */
        BSTElem<>* found = bst->searchFor(buffer);
        if (found == nullptr) responseHeader.setString("Couldn't find word");
        else responseHeader.setString('\"' + found->getData() + "\" was found " + to_string(found->getCount()) + " times");
        
        responseHeader.setPosition(
            {centerStrInBounds({responseHeader.getLocalBounds().width, responseHeader.getLocalBounds().height}, {winW, 0.f}).x, 256.f}
        );
        textbox->setBuffer();
    });
    
    topTen.setHoverAction([&window, &textBoxCursor](TextBox_0* textbox) -> void {window.setMouseCursor(textBoxCursor);});
    topTen.setUnHoverAction([&window, &normalCursor](TextBox_0* textbox) -> void {window.setMouseCursor(normalCursor);});
    topTen.setEnterAction([&bst, &responseHeader](TextBox_0* textbox, string buffer) -> void {
        /**
         * Print the top ten to a file.
         */
        vector<BSTElem<>*> traversal = bst->traverse(BST<>::inorder);
        sort(traversal.begin(), traversal.end(), [](BSTElem<>* i, BSTElem<>* j) -> bool {return i->getCount() > j->getCount();});
        ofstream outputFile;
        outputFile.open(buffer);
        if (!outputFile.is_open()) {
            responseHeader.setString("Failed to open file");
        } else {
            for (int i = 0; i < 10 && i < traversal.size(); i++) {
                outputFile << '\"' << traversal.at(i)->getData() << "\" was found " << traversal.at(i)->getCount() << " times.\n";
            }
            outputFile.close();
            responseHeader.setString("Output is located in " + buffer);
        }
        textbox->setBuffer();
    });

    wordDelete.setHoverAction([&window, &textBoxCursor](TextBox_0* textbox) -> void {window.setMouseCursor(textBoxCursor);});
    wordDelete.setUnHoverAction([&window, &normalCursor](TextBox_0* textbox) -> void {window.setMouseCursor(normalCursor);});
    wordDelete.setEnterAction([&bst, &responseHeader, &fileDataDisplay, &order, &scrollDisplay, &font]
    (TextBox_0* textbox, string buffer) -> void {
        /**
         * Delete the specified element.
         */
        bool success = bst->deleteElement(buffer);
        if (success) responseHeader.setString("Deleted succesffully.");
        else responseHeader.setString("Failed to delete word. Try again");
        textbox->setBuffer();

        vector<BSTElem<>*> traversal;
        if (order == "preorder") traversal = bst->traverse(BST<>::preorder);
        else if (order == "inorder") traversal = bst->traverse(BST<>::inorder);
        else if (order == "postorder") traversal = bst->traverse(BST<>::postorder);
        else throw "Error determining order of traversal.\n";
        convertToVectOfElems(font, traversal, scrollDisplay);
        fileDataDisplay.setData(&scrollDisplay);
        fileDataDisplay.recomputeInternalData();
    });

    /**
     * Set default actions for hover and click, etc.
     */
    searchWord.setHoverAction([](Button<>* button) -> void {button->setFillColor(sWhite, true);});
    printTopTen.setHoverAction([](Button<>* button) -> void {button->setFillColor(sWhite, true);});
    deleteWord.setHoverAction([](Button<>* button) -> void {button->setFillColor(sWhite, true);});
    displayPreOrder.setHoverAction([](Button<>* button) -> void {button->setFillColor(sWhite, true);});
    displayInOrder.setHoverAction([](Button<>* button) -> void {button->setFillColor(sWhite, true);});
    displayPostOrder.setHoverAction([](Button<>* button) -> void {button->setFillColor(sWhite, true);});
    exitProgram.setHoverAction([](Button<>* button) -> void {button->setFillColor(sWhite, true);});

    searchWord.setUnHoverAction([](Button<>* button) -> void {button->revert();});
    printTopTen.setUnHoverAction([](Button<>* button) -> void {button->revert();});
    deleteWord.setUnHoverAction([](Button<>* button) -> void {button->revert();});
    displayPreOrder.setUnHoverAction([](Button<>* button) -> void {button->revert();});
    displayInOrder.setUnHoverAction([](Button<>* button) -> void {button->revert();});
    displayPostOrder.setUnHoverAction([](Button<>* button) -> void {button->revert();});
    exitProgram.setUnHoverAction([](Button<>* button) -> void {button->revert();});

    searchWord.setClickAction([](Button<>* button) -> void {button->defaultClickEffect();});
    printTopTen.setClickAction([](Button<>* button) -> void {button->defaultClickEffect();});
    deleteWord.setClickAction([](Button<>* button) -> void {button->defaultClickEffect();});
    displayPreOrder.setClickAction([](Button<>* button) -> void {button->defaultClickEffect();});
    displayInOrder.setClickAction([](Button<>* button) -> void {button->defaultClickEffect();});
    displayPostOrder.setClickAction([](Button<>* button) -> void {button->defaultClickEffect();});
    exitProgram.setClickAction([](Button<>* button) -> void {button->defaultClickEffect();});

    /**
     * When the mouse is released over the button, search for the word specified in the input field.
     */
    searchWord.setUnClickAction([&bst, &wordSearch, &responseHeader](Button<>* button) -> void {
        button->revert();
        string word = wordSearch.getBuffer();
        BSTElem<>* found = bst->searchFor(word);
        if (found == nullptr) responseHeader.setString("Couldn't find word");
        else responseHeader.setString("Found: \"" + found->getData() + "\" with a total of " + to_string(found->getCount()) + " occurences");
        
        responseHeader.setPosition(
            {centerStrInBounds({responseHeader.getLocalBounds().width, responseHeader.getLocalBounds().height}, {winW, 0.f}).x, 256.f}
        );
    });

    /**
     * When the mouse is released over the button, print the top ten words by frequency in the file specified in the input field. 
     */
    printTopTen.setUnClickAction([&bst, &topTen, &responseHeader](Button<>* button) -> void {
        button->revert();
        vector<BSTElem<>*> traversal = bst->traverse(BST<>::inorder);
        sort(traversal.begin(), traversal.end(), [](BSTElem<>* i, BSTElem<>* j) -> bool {return i->getCount() > j->getCount();});
        ofstream outputFile;
        string buffer = topTen.getBuffer();
        outputFile.open(buffer);
        if (!outputFile.is_open()) {
            responseHeader.setString("Failed to open file");
        } else {
            for (int i = 0; i < 10 && i < traversal.size(); i++) {
                outputFile << '\"' << traversal.at(i)->getData() << "\" was found " << traversal.at(i)->getCount() << " times.\n";
            }
            outputFile.close();
            responseHeader.setString("Output is located in " + buffer);
        }
        topTen.setBuffer();
    });

    /**
     * When the mouse is released over the button, delete the element specified in the input field.
     */
    deleteWord.setUnClickAction([&bst, &wordDelete, &responseHeader, &fileDataDisplay, &order, &scrollDisplay, &font]
    (Button<>* button) -> void {
        bool success = bst->deleteElement(wordDelete.getBuffer());
        if (success) responseHeader.setString("Deleted succesffully.");
        else responseHeader.setString("Failed to delete word. Try again");
        wordDelete.setBuffer();

        vector<BSTElem<>*> traversal;
        if (order == "preorder") traversal = bst->traverse(BST<>::preorder);
        else if (order == "inorder") traversal = bst->traverse(BST<>::inorder);
        else if (order == "postorder") traversal = bst->traverse(BST<>::postorder);
        else throw "Error determining order of traversal.\n";
        convertToVectOfElems(font, traversal, scrollDisplay);
        fileDataDisplay.setData(&scrollDisplay);
        fileDataDisplay.recomputeInternalData();
    });

    /**
     * Display the elements in preorder order.
     */
    displayPreOrder.setUnClickAction([&bst, &fileDataDisplay, &scrollDisplay, &font, &defaultOrderDisplay, &headerContent, &order]
    (Button<>* button) -> void {
        button->revert();
        vector<BSTElem<>*> traversal = bst->traverse(BST<>::preorder);
        convertToVectOfElems(font, traversal, scrollDisplay);
        fileDataDisplay.setData(&scrollDisplay);
        headerContent.setString(defaultOrderDisplay + (order = "preorder"));
    });

    /**
     * Display the elements in inorder order.
     */
    displayInOrder.setUnClickAction([&bst, &fileDataDisplay, &scrollDisplay, &font, &defaultOrderDisplay, &headerContent, &order]
    (Button<>* button) -> void {
        button->revert();
        vector<BSTElem<>*> traversal = bst->traverse(BST<>::inorder);
        convertToVectOfElems(font, traversal, scrollDisplay);
        fileDataDisplay.setData(&scrollDisplay);
        headerContent.setString(defaultOrderDisplay + (order = "inorder"));
    });

    /**
     * Display the elements in postorder order.
     */
    displayPostOrder.setUnClickAction([&bst, &fileDataDisplay, &scrollDisplay, &font, &defaultOrderDisplay, &headerContent, &order]
    (Button<>* button) -> void {
        button->revert();
        vector<BSTElem<>*> traversal = bst->traverse(BST<>::postorder);
        convertToVectOfElems(font, traversal, scrollDisplay);
        fileDataDisplay.setData(&scrollDisplay);
        headerContent.setString(defaultOrderDisplay + (order = "postorder"));
    });

    /**
     * Exit the program.
     */
    exitProgram.setUnClickAction([&window](Button<>* button) -> void {
        button->revert();
        window.close();
    });

    currentScene = &enterFile; // Initialize current scene

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

                for (Focus* val : currentScene->focusable) if (val->isMouseOver(mousePos)) {
                    focus = val;
                    break;
                }
                for (Hold* val : currentScene->holdable) if (val->isMouseOver(mousePos)) {
                    hold = val;
                    break;
                }
                for (Hover* val : currentScene->hoverable) if (val->isMouseOver(mousePos)) {
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
                for (Hover* val : currentScene->hoverable) {
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
        window.clear();
        if (currentScene != nullptr) window.draw(*currentScene);
        window.display();
    }
    FreeConsole();
    return 0;
}