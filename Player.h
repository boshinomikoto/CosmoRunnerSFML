#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"

class Player
{
public:
    Player() : sprite(sf::Vector2f(88, 200))
    {
        playerTexture.loadFromFile("IMG/SHIP/broomhilda.png");
        sprite.setTexture(&playerTexture);
        sprite.setPosition(184, 600);
        herotexture.loadFromFile("IMG/SHIP/broomhilda_animation.png");
        herosprite.setTexture(herotexture);
    }

    void shipControl()
    {
        targetSpeed = sf::Vector2f(0.0f, 0.0f);
        float speed = 500.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    targetSpeed.y = -speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  targetSpeed.y = speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  targetSpeed.x = -speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) targetSpeed.x = speed;
    }

    void moveShip(float dt)
    {
        float acceleration = 8.f;
        currentSpeed.x += (targetSpeed.x - currentSpeed.x) * acceleration * dt;
        currentSpeed.y += (targetSpeed.y - currentSpeed.y) * acceleration * dt;
        sprite.move(currentSpeed * dt);

        sf::Vector2f pos = sprite.getPosition();
        if (pos.x > WIDTH - 90)   sprite.setPosition(WIDTH - 90, pos.y);
        if (pos.x < 5)            sprite.setPosition(5, pos.y);
        if (pos.y > HEIGHT - 355) sprite.setPosition(pos.x, HEIGHT - 355);
        if (pos.y < 5)            sprite.setPosition(pos.x, 5);

        if ((pos.x > WIDTH - 90) && (pos.y < 5))             sprite.setPosition(WIDTH - 90, 5);
        if ((pos.x > WIDTH - 90) && (pos.y > HEIGHT - 355))  sprite.setPosition(WIDTH - 90, HEIGHT - 355);
        if ((pos.x < 5) && (pos.y > HEIGHT - 355))           sprite.setPosition(5, HEIGHT - 355);
        if ((pos.x < 5) && (pos.y < 5))                      sprite.setPosition(5, 5);
    }

    Pos GetCurrentPos()
    {
        Pos result;
        sf::Vector2f pos = sprite.getPosition();
        result.x = pos.x;
        result.y = pos.y;
        return result;
    }

    void SetCurrentPosition() { sprite.setPosition(184, 600); }

    void AnimJet()
    {
        if (clockAnim.getElapsedTime() > sf::milliseconds(100))
        {
            clockAnim.restart();
            Frame += stepanim;
            herosprite.setTextureRect(sf::IntRect(arrNumY[Frame], 0, 61, 52));
            if (Frame == 0) stepanim = 1;
            if (Frame == 2) stepanim = -1;
        }
        herosprite.setPosition(sf::Vector2f(sprite.getPosition().x + 17, sprite.getPosition().y + 181));
    }

    sf::FloatRect getBounds()
    {
        sf::FloatRect bounds = sprite.getGlobalBounds();
        float sx = bounds.width * 0.3f;
        return sf::FloatRect(bounds.left + sx, bounds.top, bounds.width - sx * 2, bounds.height);
    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(herosprite);
        window.draw(sprite);
    }

private:
    sf::Vector2f       targetSpeed;
    sf::Vector2f       currentSpeed;
    sf::Texture        playerTexture;
    sf::RectangleShape sprite;
    sf::Texture        herotexture;
    sf::Sprite         herosprite;
    static constexpr int arrNumY[3]{ 0, 62, 120 };
    int Frame = 3, stepanim = -1;
    sf::Clock clockAnim;
};