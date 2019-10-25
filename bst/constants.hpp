#define SFML_STATIC

#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <SFML/Graphics.hpp>

using namespace std;

const int
    textSize = 16,
    buttonHeight = 20,
    winW = 512, winH = 384;

const float
    defX = winW / 2, defY = winH / 2;

const string testChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ 1234567890`-=~!@#$%^&*()_+[]\\{}|;':\",./<>?";

const sf::Color
    lGrey(191, 191, 191),
    sBlack(34, 34, 34),
    dGrey(63, 63, 63),
    sWhite(223, 223, 223),
    grey(127, 127, 127);

#endif