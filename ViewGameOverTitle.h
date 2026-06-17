#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"

class ViewGameOverTitle
{
public:
    ViewGameOverTitle()
    {
        TextureObject.loadFromFile("IMG/GAMEOVER/gameover.png");
        title.setTexture(TextureObject);
        title.setTextureRect(sf::IntRect(0, 0, 292, 150));
        title.setPosition((WIDTH / 2 - (292 / 2)), HEIGHT / 2 - 150);
    }

    void hide() { title.setColor(sf::Color(255, 255, 255, 0)); }
    void show() { title.setColor(sf::Color(255, 255, 255, 255)); }
    void draw(sf::RenderWindow& window) { window.draw(title); }

private:
    sf::Sprite  title;
    sf::Texture TextureObject;
};