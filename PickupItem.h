#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <cstdlib>
#include "Constants.h"

class PickupItem
{
public:
    PickupItem(const std::string& texturePath, sf::IntRect texRect,
        int marginX, float yMultiplier, int speedMin, int speedMax)
        : marginX(marginX), yMultiplier(yMultiplier),
        speedMin(speedMin), speedMax(speedMax)
    {
        texture.loadFromFile(texturePath);
        sprite.setTexture(texture);
        sprite.setTextureRect(texRect);
        spawn();
    }

    void spawn()
    {
        float x = static_cast<float>(rand() % (int)(WIDTH - marginX - marginX + 1) + marginX);
        float y = -yMultiplier * static_cast<float>(rand() % (int)HEIGHT) - 180.f;
        sprite.setPosition(x, y);
        speedY = static_cast<float>(rand() % (speedMax - speedMin) + speedMin);
    }

    void update(float dt)
    {
        sprite.move(0.f, speedY * dt);
        if (sprite.getPosition().y > HEIGHT - 190) spawn();
    }

    void draw(sf::RenderWindow& window) { window.draw(sprite); }

    bool collision(sf::FloatRect playerBounds) const
    {
        return sprite.getGlobalBounds().intersects(playerBounds);
    }

protected:
    sf::Sprite  sprite;
    sf::Texture texture;
    float       speedY = 0.f;

private:
    int   marginX;
    float yMultiplier;
    int   speedMin;
    int   speedMax;
};