#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cstdlib>
#include "Constants.h"
#include "AlgorithmiCalculations.h"

class SpaceTrash
{
public:
    SpaceTrash()
    {
        shuffle.FisherYatesShuffle(images);
        TextureObject.loadFromFile("IMG/trash/" + images[0]);
        SpaceObject.setTexture(TextureObject);
        SpaceObject.setTextureRect(sf::IntRect(0, 0, 121, 110));
        SpaceObject.setOrigin(55, 55);
        respawn();
    }

    void respawn()
    {
        float x = static_cast<float>(rand() % (int)WIDTH);
        float y = -180.f;
        SpaceObject.setPosition(x, y);
        float scale = static_cast<float>(rand() % 7 + 5) / 10.f;
        SpaceObject.setScale(scale, scale);
        speedY = static_cast<float>(rand() % 200 + 100);
        rotationSpeed = static_cast<float>(rand() % 121 - 60);
    }

    void update(float dt)
    {
        SpaceObject.move(0.0f, speedY * dt);
        SpaceObject.rotate(rotationSpeed * dt);
        if (SpaceObject.getPosition().y > HEIGHT - 190)
            respawn();
    }

    void draw(sf::RenderWindow& window) { window.draw(SpaceObject); }

    bool collision(sf::FloatRect playerBounds) const
    {
        sf::FloatRect bounds = SpaceObject.getGlobalBounds();
        float sx = bounds.width  * 0.2f;
        float sy = bounds.height * 0.2f;
        sf::FloatRect hitbox(bounds.left + sx, bounds.top + sy, bounds.width - sx * 2, bounds.height - sy * 2);
        return hitbox.intersects(playerBounds);
    }

private:
    std::vector<std::string> images{ "1.png", "2.png", "5.png", "6.png", "8_1.png", "8_2.png", "8_3.png" };
    AlgorithmiCalculations   shuffle;
    sf::Sprite  SpaceObject;
    sf::Texture TextureObject;
    float       speedY = 150.f;
    float       rotationSpeed = 0.f;
};

class TrashManager
{
public:
    TrashManager() { meteors.resize(COUNT); }

    bool update(float dt, sf::FloatRect playerBounds)
    {
        bool hit = false;
        for (auto& m : meteors)
        {
            m.update(dt);
            if (m.collision(playerBounds)) hit = true;
        }
        return hit;
    }

    void draw(sf::RenderWindow& window) { for (auto& m : meteors) m.draw(window); }
    void reset() { for (auto& m : meteors) m.respawn(); }

private:
    static constexpr int COUNT = 3;
    std::vector<SpaceTrash> meteors;
};