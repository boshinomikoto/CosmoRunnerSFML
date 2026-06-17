#pragma once
#include <vector>
#include "PickupItem.h"

class Fuel : public PickupItem
{
public:
    Fuel() : PickupItem("IMG/FUEL/fuel.png", sf::IntRect(0, 0, 38, 50), 38, 2.f, 100, 300) {}
};

class FuelManager
{
public:
    FuelManager() { objects.resize(COUNT); }

    bool update(float dt, sf::FloatRect playerBounds)
    {
        bool hit = false;
        for (auto& obj : objects)
        {
            obj.update(dt);
            if (obj.collision(playerBounds)) { obj.spawn(); hit = true; }
        }
        return hit;
    }

    void draw(sf::RenderWindow& window) { for (auto& obj : objects) obj.draw(window); }
    void reset() { for (auto& obj : objects) obj.spawn(); }

private:
    static constexpr int COUNT = 3;
    std::vector<Fuel> objects;
};