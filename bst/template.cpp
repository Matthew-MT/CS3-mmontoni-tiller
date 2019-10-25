/**
 * Compile with:
 * g++ -o test0.exe test0.cpp -Isfml/include -Lsfml/lib "-Wl,--start-group" -lfreetype -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lsfml-main -lgdi32 -lopengl32 -lwinmm "-Wl,--end-group"
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
#include <iostream>
#include <cstdlib>
#include "pop.hpp"
#include <string>

#include "bst.hpp"

using namespace std;

int main(int argCount, char* args[]) {
    if (argCount > 1 && (string)args[1] == "console");
    else ShowWindow(FindWindowA("ConsoleWindowClass", NULL), false);

    sf::Clock clock;
    Focus* focus = nullptr;
    Hold* hold = nullptr;

    vector<Focus*> focusable;
    vector<Hold*> holdable;
    vector<Hover*> hoverable;
    vector<sf::Drawable*> drawable;

    sf::RenderWindow window(sf::VideoMode(winW, winH), "Binary Search Tree", sf::Style::Default);
    sf::Font font;
    if (!font.loadFromFile("./fonts/cour.ttf")) throw "Couldn't find font";

    sf::RectangleShape whiteSpace;
    whiteSpace.setSize({winW, winH});
    drawable.push_back(&whiteSpace);
    PopBackground background({0, 0}, {winW, winH});
    background.setPadding(2.f);
    background.invert(true);
    drawable.push_back(&background);

    sf::Cursor normalCursor, textBoxCursor;
    normalCursor.loadFromSystem(sf::Cursor::Arrow);
    textBoxCursor.loadFromSystem(sf::Cursor::Text);

    BST<string>* bst = new BST<string>();
    {
        //no need to set search functions, ">" and "<" are already defined properly for strings
        for (int i = 0; i < 100; i++) bst->insertElement("Element " + to_string(rand() % 100));
    }

    {
        vector<BSTElem<string>*> elements = bst->traverse(BST<>::inorder);
        for (BSTElem<string>* val : elements) {
            string children = "\n";
            if (val->left() != nullptr) children += "L Left child: " + val->left()->getData() + '\n';
            if (val->right() != nullptr) children += "L Right child: " + val->right()->getData() + '\n';

            cout << val->getData() << children << '\n';
        }
    }

    {
        for (int i = 0;; i++) {
            cout << "================\n";
            BSTElem<string>* elem = bst->searchFor("Element " + to_string(i));
            if (elem != nullptr) {
                cout << "Element found: " << elem->getData() << '\n';
                bst->deleteElement(elem->getData());
                cout << "All the way back here\n";

                /*{
                    vector<BSTElem<string>*> elements = bst->traverse(BST<>::inorder);
                    for (BSTElem<string>* val : elements) cout << val->getData() << '\n';
                }*/

                //cout << bst->searchFor("Element 0")->getData() << '\n';

                //elem = bst->searchFor("Element " + to_string(i));
                //if (elem == nullptr) cout << "Deletion successful\n";
                //else cout << "Failed to delete: " << elem->getData() << '\n';
                break;
            } else cout << "Fail: " << i << '\n';
        }
    }

    vector<Element> dataVector = {
        {font, "Element 0"},
        {font, "Element 1"},
        {font, "Element 2"},
        {font, "Element 3"},
        {font, "Element 4"},
        {font, "Element 5"},
        {font, "Element 6"},
        {font, "Element 7"},
        {font, "Element 8"},
        {font, "Element 9"}
    };
    /*ScrollWindow_0<Element> myScrollWindow(&dataVector, {32.f, 32.f}, {128.f, 192.f});

    myScrollWindow.setHoverAction([](Scroll* scrollbar) -> void {
        scrollbar->setFillColor(lGrey, true);
    });

    myScrollWindow.setUnHoverAction([](Scroll* scrollbar) -> void {
        scrollbar->revert();
    });

    myScrollWindow.setClickAction([](Scroll* scrollbar) -> void {
        scrollbar->invert(true);
    });

    myScrollWindow.setUnClickAction([](Scroll* scrollbar) -> void {
        scrollbar->revert();
    });*/

    sf::Text header("Please input file name below:", font, 28);
    {
        sf::FloatRect headerBounds = header.getLocalBounds();
        header.setPosition(centerStrInBounds({headerBounds.width, headerBounds.height}, {winW, winH * 0.8}));
        header.setFillColor(sBlack);
        drawable.push_back(&header);
    }

    TextBox_0 fileInput(font, 16, {(winW / 2.f) - 64.f, winH / 2.f}, {2.f, 2.f}, {128.f, -1.f});
    {
        fileInput.setClickAction([](TextBox_0* textbox) -> void {});
        fileInput.setUnClickAction([](TextBox_0* textbox) -> void {});

        fileInput.setHoverAction([&window, &textBoxCursor](TextBox_0* textbox) -> void {
            window.setMouseCursor(textBoxCursor);
        });

        fileInput.setUnHoverAction([&window, &normalCursor](TextBox_0* textbox) -> void {
            window.setMouseCursor(normalCursor);
        });

        focusable.push_back(&fileInput);
        hoverable.push_back(&fileInput);
        drawable.push_back(&fileInput);
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            sf::Vector2i tempMousePos = sf::Mouse::getPosition(window);
            sf::Vector2f mousePos = {(float)tempMousePos.x, (float)tempMousePos.y};
            if (event.type == sf::Event::Closed) window.close();
            else if (event.type == sf::Event::MouseButtonPressed) {
                for (Focus* val : focusable) if (val->isMouseOver(mousePos)) {
                    focus = val;
                    break;
                }
                for (Hold* val : holdable) if (val->isMouseOver(mousePos)) {
                    hold = val;
                    break;
                }

                if (focus != nullptr) {
                    if (!focus->isMouseOver(mousePos)) focus->endFocus(), focus = nullptr;
                    else focus->startFocus(clock.getElapsedTime()), focus->click();
                }
                if (hold != nullptr) hold->startHold(mousePos), hold->click();
            } else if (event.type == sf::Event::MouseButtonReleased) {
                if (hold != nullptr) {
                    hold->unClick();
                    hold->endHold();
                    hold = nullptr;
                }
                if (focus != nullptr) focus->unClick();
            } else if (event.type == sf::Event::MouseMoved) {
                if (hold != nullptr && sf::Mouse::isButtonPressed(sf::Mouse::Left)) hold->updateHold(mousePos);
            } else if (event.type == sf::Event::KeyReleased) {
                if (focus != nullptr) focus->registerKeystroke(
                    event.key.code,
                    sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)
                );
            }
        }
        sf::Time time = clock.getElapsedTime();
        if (focus != nullptr) focus->focusTick(time);
        window.clear();
        for (sf::Drawable* val : drawable) window.draw(*val);
        /*window.draw(whiteSpace);
        window.draw(background);
        window.draw(myScrollWindow);
        window.draw(fileInput);*/
        window.display();
    }
    FreeConsole();
    return 0;
}