#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Constants.h"
#include "Player.h"
#include "ShipDamage.h"
#include "SpaceTrash.h"
#include "Health.h"
#include "ViewGameOverTitle.h"
#include "FuelIndicator.h"
#include "Fuel.h"
#include "Enemies.h"
#include "AddictiveLive.h"
#include "PauseMenu.h"
#include "Records.h"
#include "AlgorithmiCalculations.h"

class Game
{
public:
    Game();
    void run();

private:
    void updateTime();
    void processEvents();
    void update();
    void render();

    sf::RenderWindow   window;
    sf::Image          icon;
    sf::Texture        TexturInfoPanel;
    sf::RectangleShape GameInfoPanel;
    sf::Texture        textureSpace;
    sf::RectangleShape gamingBackground;
    sf::RectangleShape gamingBackground2;
    sf::Vector2f       pos;
    sf::Clock          clock;
    float              dt;

    Player player;

    sf::Font font;
    sf::Text text, text1, text2, text3, bestText;

    int score = 0;

    TrashManager         trashManager;
    FuelManager          fuelManager;
    FuelIndicator        indicator;
    AddictiveLiveManager addLive;
    EnemManager          enemManager{ 184.f, 600.f };

    bool gameOver = false;

    AlgorithmiCalculations way;

    Health health[3]
    {
        Health(291, HEIGHT - 121, 33, 16),
        Health(326, HEIGHT - 121, 34, 16),
        Health(361, HEIGHT - 121, 35, 16)
    };

    int alive = 3;
    int speed = 0;

    sf::Clock  timer;
    sf::Music  music;
    sf::Music  explosion_sound;
    ShipDamage expl;

    ViewGameOverTitle gameOverText;
    PauseMenu         pauseMenu{ font };

    Records  records;
    bool     explosionPlayed = false;
    bool     paused = false;
};