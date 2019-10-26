#define SFML_STATIC

#ifndef SCENE_HPP
#define SCENE_HPP

#include <SFML/Graphics.hpp>
#include "concrete.hpp"
#include <functional>
#include <iostream>

using namespace std;

class Scene : public sf::Drawable {
public:
    function<void(Scene* scene)> onSceneSwitchTo, onSceneSwitchAway;
    vector<Focus*> focusable;
    vector<Hold*> holdable;
    vector<Hover*> hoverable;
    vector<sf::Drawable*> drawable;
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        for (sf::Drawable* val : drawable) target.draw(*val, states);
    }
public:
    Scene(
        vector<Focus*> focusVect = {},
        vector<Hold*> holdVect = {},
        vector<Hover*> hoverVect = {},
        vector<sf::Drawable*> drawVect = {}
    );

    void setSwitchToAction(function<void(Scene* scene)> fnc);
    void setSwitchAwayAction(function<void(Scene* scene)> fnc);

    void switchTo();
    void switchAway();
};

Scene::Scene(
    vector<Focus*> focusVect,
    vector<Hold*> holdVect,
    vector<Hover*> hoverVect,
    vector<sf::Drawable*> drawVect
) {
    focusable = focusVect;
    holdable = holdVect;
    hoverable = hoverVect;
    drawable = drawVect;
}

void Scene::setSwitchToAction(function<void(Scene* scene)> fnc) {onSceneSwitchTo = fnc;}
void Scene::setSwitchAwayAction(function<void(Scene* scene)> fnc) {onSceneSwitchAway = fnc;}

void Scene::switchTo() {if ((bool)onSceneSwitchTo) onSceneSwitchTo(this);}
void Scene::switchAway() {if ((bool)onSceneSwitchAway) onSceneSwitchAway(this);}

#endif