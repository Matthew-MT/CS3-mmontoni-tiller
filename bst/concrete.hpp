#define SFML_STATIC

#ifndef FOCUSABLE_HPP
#define FOCUSABLE_HPP

#include <SFML/Graphics.hpp>
#include <functional>

using namespace std;

class Concrete : public sf::Drawable {
public:
    virtual sf::Vector2f getPosition() = 0;
    virtual sf::Vector2f getSize() = 0;
};

class Hover : public Concrete {
public:
    virtual bool isMouseOver(const sf::Vector2f& mousePos);
    virtual void hover() = 0;
    virtual void unHover() = 0;
    virtual void click() = 0;
    virtual void unClick() = 0;
};

bool Hover::isMouseOver(const sf::Vector2f& mousePos) {
    sf::Vector2f pos = this->getPosition(), size = this->getSize();
    return (sf::FloatRect{pos.x, pos.y, size.x, size.y}).contains(mousePos);
}

class Focus : public Hover {
public:
    virtual void startFocus(const sf::Time& time) = 0;
    virtual void focusTick(const sf::Time& time) = 0;
    virtual void endFocus() = 0;
    virtual void registerKeystroke(const sf::Keyboard::Key& key, bool shiftPressed) {throw "Invalid call";}
};

class Hold : public Hover {
public:
    virtual void startHold(const sf::Vector2f& mousePos) = 0;
    virtual void updateHold(const sf::Vector2f& mousePos) = 0;
    virtual void endHold() = 0;
};

#endif