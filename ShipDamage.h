#pragma once
#include <SFML/Graphics.hpp>

class ShipDamage
{
public:
    ShipDamage()
    {
        texture.loadFromFile("IMG/SHIP/spritesheetCopy.png");
        sprite.setTexture(texture);
        sprite.setColor(sf::Color::Transparent);
    }

    void animExplosion(float x, float y)
    {
        if (clock.getElapsedTime() > sf::milliseconds(100))
        {
            sprite.setColor(sf::Color::White);
            clock.restart();
            if (Frame < 38) Frame += stepanim;
        }
        sprite.setTextureRect(sf::IntRect(arrNumX[Frame], 0, 341, 300));
        sprite.setPosition(x, y);
    }

    void reset()
    {
        Frame = 0;
        sprite.setColor(sf::Color::Transparent);
    }

    void draw(sf::RenderWindow& window) { window.draw(sprite); }

private:
    sf::Sprite  sprite;
    sf::Texture texture;
    static constexpr int arrNumX[39] = {
        0, 341, 682, 1023, 1364, 1705, 2046, 2387, 2728, 3069,
        3410, 3751, 4092, 4433, 4774, 5115, 5456, 5797, 6138, 6479,
        6820, 7161, 7502, 7843, 8184, 8525, 8866, 9207, 9548, 9889,
        10230, 10571, 10912, 11253, 11594, 11935, 12276, 12610, 13020
    };
    int Frame = 0, stepanim = 1;
    sf::Clock clock;
};