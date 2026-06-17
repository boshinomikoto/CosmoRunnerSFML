#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"

class Health
{
public:
    Health(int X, int Y, int sizeX, int sizeY)
    {
        TextureObject.loadFromFile("IMG/Panel/health.png");
        Hearts.setTexture(TextureObject);
        Hearts.setTextureRect(sf::IntRect(0, 0, sizeX, sizeY));
        Hearts.setPosition(X, Y);
    }

    void draw(sf::RenderWindow& window) { window.draw(Hearts); }
    void destroy() { Hearts.setColor(sf::Color(255, 255, 255, 0)); }
    void resetHearts() { Hearts.setColor(sf::Color(255, 255, 255, 255)); }

private:
    sf::Sprite  Hearts;
    sf::Texture TextureObject;
};