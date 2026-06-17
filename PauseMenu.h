#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Constants.h"

class PauseMenu
{
public:
    PauseMenu(sf::Font& font)
    {
        overlay.setSize(sf::Vector2f(WIDTH, HEIGHT));
        overlay.setFillColor(sf::Color(0, 0, 0, 150));

        title.setFont(font);
        title.setCharacterSize(30);
        title.setFillColor(sf::Color::White);
        title.setString("PAUSE");
        centerText(title, HEIGHT * 0.3f);

        initButton(btnResume, font, "RESUME", HEIGHT * 0.45f);
        initButton(btnRestart, font, "RESTART", HEIGHT * 0.55f);
        initButton(btnExit, font, "EXIT", HEIGHT * 0.65f);
    }

    enum class Action { None, Resume, Restart, Exit };

    Action handleClick(sf::Vector2i mousePos)
    {
        sf::Vector2f mp = static_cast<sf::Vector2f>(mousePos);
        if (btnResume.getGlobalBounds().contains(mp))  return Action::Resume;
        if (btnRestart.getGlobalBounds().contains(mp)) return Action::Restart;
        if (btnExit.getGlobalBounds().contains(mp))    return Action::Exit;
        return Action::None;
    }

    void handleHover(sf::Vector2i mousePos)
    {
        sf::Vector2f mp = static_cast<sf::Vector2f>(mousePos);
        btnResume.setFillColor(btnResume.getGlobalBounds().contains(mp) ? sf::Color::Yellow : sf::Color::White);
        btnRestart.setFillColor(btnRestart.getGlobalBounds().contains(mp) ? sf::Color::Yellow : sf::Color::White);
        btnExit.setFillColor(btnExit.getGlobalBounds().contains(mp) ? sf::Color::Yellow : sf::Color::White);
    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(overlay);
        window.draw(title);
        window.draw(btnResume);
        window.draw(btnRestart);
        window.draw(btnExit);
    }

private:
    void initButton(sf::Text& btn, sf::Font& font, const std::string& label, float y)
    {
        btn.setFont(font);
        btn.setCharacterSize(28);
        btn.setFillColor(sf::Color::White);
        btn.setString(label);
        centerText(btn, y);
    }

    void centerText(sf::Text& t, float y)
    {
        sf::FloatRect b = t.getLocalBounds();
        t.setOrigin(b.width / 2.f, b.height / 2.f);
        t.setPosition(WIDTH / 2.f - 100.f, y - 50.f);
    }

    sf::RectangleShape overlay;
    sf::Text title, btnResume, btnRestart, btnExit;
};