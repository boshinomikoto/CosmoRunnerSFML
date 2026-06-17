#pragma once
#include <vector>
#include "PickupItem.h"

class AddictiveLive : public PickupItem
{
public:
    AddictiveLive() : PickupItem("IMG/heart/add_heart.png", sf::IntRect(0, 0, 59, 50), 60, 5.f, 160, 360) {}
};

class AddictiveLiveManager
{
public:
    AddictiveLiveManager() { objects.resize(COUNT); }

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
    static constexpr int COUNT = 1;
    std::vector<AddictiveLive> objects;
};