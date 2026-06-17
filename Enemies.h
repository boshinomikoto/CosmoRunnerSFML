#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <cstdlib>
#include "Constants.h"
#include "AlgorithmiCalculations.h"

class Enemies
{
public:
    Enemies(float playerX, float playerY)
    {
        TextureObject.loadFromFile("IMG/Enemies/drone1.png");
        Enem.setTexture(TextureObject);
        Enem.setTextureRect(sf::IntRect(0, 0, 75, 97));
        spawn(playerX, playerY);
    }

    void hide() { Enem.setPosition(-1000.f, -1000.f); velocity = { 0.f, 0.f }; }

    void spawn(float playerX, float playerY)
    {
        int min_valX = WIDTH - 150;
        int max_valX = WIDTH + 150;
        float x = ALG.mergeTwoUnrelatedRanges(min_valX, -51, WIDTH + 50, max_valX);

        int min_valY = 0;
        int max_valY = HEIGHT - 350;
        float y = static_cast<float>(min_valY + rand() % (max_valY - min_valY + 1));
        Enem.setPosition(x, y);

        sf::Vector2f dir(playerX - x, playerY - y);
        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (len > 0) dir /= len;

        float angle = std::atan2(dir.y, dir.x) * 180.f / 3.14159265f;
        Enem.setRotation(angle);
        float spd = static_cast<float>(rand() % 220 + 200);
        velocity = dir * spd;
    }

    bool update(float dt, float playerX, float playerY)
    {
        Enem.move(velocity * dt);
        if (Enem.getPosition().y+97 > HEIGHT - 100 || Enem.getPosition().x > WIDTH + 160 || Enem.getPosition().x < -160)
        {
            spawn(playerX, playerY);
            return true;
        }
        return false;
    }

    void draw(sf::RenderWindow& window) { window.draw(Enem); }

    bool collision(sf::FloatRect playerBounds) const
    {
        sf::FloatRect bounds = Enem.getGlobalBounds();
        float sx = bounds.width * 0.1f;
        float sy = bounds.height * 0.1f;
        sf::FloatRect hitbox(bounds.left + sx, bounds.top + sy, bounds.width - sx * 2, bounds.height - sy * 2);
        return hitbox.intersects(playerBounds);
    }

private:
    AlgorithmiCalculations ALG;
    sf::Sprite   Enem;
    sf::Texture  TextureObject;
    sf::Vector2f velocity;
};

class EnemManager
{
public:
    EnemManager(float playerX, float playerY)
    {
        objects.reserve(COUNT);
        for (int i = 0; i < COUNT; i++)
            objects.emplace_back(playerX, playerY);
    }

    bool update(float dt, sf::FloatRect playerBounds, float playerX, float playerY)
    {
        bool hit = false;
        if (waiting)
        {
            if (respawnTimer.getElapsedTime() > sf::seconds(2.f))
            {
                waiting = false;
                for (auto& obj : objects) obj.spawn(playerX, playerY);
            }
            return false;
        }
        for (auto& obj : objects)
        {
            bool escaped = obj.update(dt, playerX, playerY);
            if (escaped) 
            {
                totalEscaped++; 
                continue; 
            }
            if (obj.collision(playerBounds))
            {
                totalEscaped++;
                obj.hide();
                waiting = true;
                respawnTimer.restart();
                hit = true;
            }
        }
        return hit;
    }

    void resetEscapeCount() { totalEscaped = 0; }
    int  getEscapeCount() const { return totalEscaped; }
    void draw(sf::RenderWindow& window) { for (auto& obj : objects) obj.draw(window); }
    void reset(float playerX, float playerY) { for (auto& obj : objects) obj.spawn(playerX, playerY); }

private:
    std::vector<Enemies> objects;
    const int COUNT = 1;
    sf::Clock respawnTimer;
    bool waiting = false;
    int  totalEscaped = 0;
};