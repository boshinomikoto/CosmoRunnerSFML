#pragma once
#include <SFML/Graphics.hpp>

class FuelIndicator
{
public:
    FuelIndicator()
    {
        TextureObject.loadFromFile("IMG/charge/fuelSpriteList2.png");
        indicator.setTexture(TextureObject);
        indicator.setPosition(452, 812);
    }

    void animFuelIndicator(int score)
    {
        if (score % 750 == 0)
            if (Frame > 0) Frame += stepanim;
        indicator.setTextureRect(sf::IntRect(arrNumX[Frame], 0, 34, 60));
    }

    void refueling() { if (Frame < 8) Frame++; }
    int  getFrame() const { return Frame; }
    void reset() { Frame = 8; }
    void draw(sf::RenderWindow& window) { window.draw(indicator); }

private:
    sf::Sprite  indicator;
    sf::Texture TextureObject;
    sf::Clock   clock;
    static constexpr int arrNumX[9] = { 0,38,77,116,153,190,227,263,299 };
    int Frame = 8, stepanim = -1;
};