#include "Game.h"

Game::Game()
    : window(sf::VideoMode(WIDTH, HEIGHT), "Cosmo Battle!"),
    GameInfoPanel(sf::Vector2f(WIDTH, 136)),
    gamingBackground(sf::Vector2f(WIDTH, HEIGHT)),
    gamingBackground2(sf::Vector2f(WIDTH, HEIGHT)),
    dt(0.0)
{
    music.openFromFile("Music/iron_tid_rising.ogg");
    music.setLoop(true);
    music.setVolume(5);
    music.play();

    explosion_sound.openFromFile("Music/0527.ogg");
    explosion_sound.setPitch(2.f);
    explosion_sound.setVolume(10);

    window.setVerticalSyncEnabled(true);
    if (!icon.loadFromFile("IMG/icon.png"))
        throw std::runtime_error("cannot load icon");
    window.setIcon(32, 32, icon.getPixelsPtr());

    TexturInfoPanel.loadFromFile("IMG/Panel/PA_NEL2-Photoroom.png");
    GameInfoPanel.setTexture(&TexturInfoPanel);
    GameInfoPanel.setPosition(0, HEIGHT - 136);

    textureSpace.loadFromFile("IMG/background/space_back2.jpg");
    gamingBackground.setTexture(&textureSpace);
    gamingBackground2.setTexture(&textureSpace);
    gamingBackground2.setPosition(0, -HEIGHT);

    if (!font.loadFromFile("fonts/LCD16x2Remastered-RegularV2.otf"))
        throw std::runtime_error("cannot load font");

    text.setFont(font);   text.setCharacterSize(15);  text.setFillColor(sf::Color::White);
    text1.setFont(font);  text1.setCharacterSize(12); text1.setFillColor(sf::Color::White);
    text2.setFont(font);  text2.setCharacterSize(10); text2.setFillColor(sf::Color::White);
    text2.setPosition(340, HEIGHT - 53); text2.setString("KM/h");
    text3.setFont(font);  text3.setCharacterSize(15); text3.setFillColor(sf::Color::White);
    text3.setPosition(83, HEIGHT - 71);
    bestText.setFont(font); bestText.setCharacterSize(10); bestText.setFillColor(sf::Color::White);
    bestText.setPosition(20, 15);
}

void Game::run()
{
    while (window.isOpen())
    {
        updateTime();
        processEvents();
        update();
        render();
    }
}

void Game::updateTime()
{
    dt = clock.restart().asSeconds();
}

void Game::processEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space && gameOver && !paused)
        {
            gameOver = false; alive = 3; score = 0; explosionPlayed = false;
            health[0].resetHearts(); health[1].resetHearts(); health[2].resetHearts();
            trashManager.reset(); fuelManager.reset(); addLive.reset();
            expl.reset(); indicator.reset();
            Pos p = player.GetCurrentPos();
            enemManager.reset(p.x, p.y);
            enemManager.resetEscapeCount();
            player.SetCurrentPosition();
            explosion_sound.stop();
        }

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        {
            if (gameOver) gameOverText.hide();
            paused = !paused;
            if (paused) { music.pause(); explosion_sound.pause(); }
            else
            {
                if (gameOver) gameOverText.show();
                music.play();
                if (!(explosion_sound.getStatus() == sf::Sound::Stopped))
                    explosion_sound.play();
            }
        }

        if (paused && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            auto action = pauseMenu.handleClick(sf::Mouse::getPosition(window));
            switch (action)
            {
            case PauseMenu::Action::Resume:
                paused = false; music.play();
                if (gameOver) gameOverText.show();
                break;

            case PauseMenu::Action::Restart:
                paused = false; gameOver = false; alive = 3; score = 0; explosionPlayed = false;
                health[0].resetHearts(); health[1].resetHearts(); health[2].resetHearts();
                trashManager.reset(); fuelManager.reset();
                enemManager.resetEscapeCount(); expl.reset(); indicator.reset();
                explosion_sound.stop(); player.SetCurrentPosition(); music.play();
                break;

            case PauseMenu::Action::Exit:
                window.close();
                break;

            default: break;
            }
        }
        if (paused) pauseMenu.handleHover(sf::Mouse::getPosition(window));
    }
}

void Game::update()
{
    if (paused) return;

    if (gameOver)
    {
        if (!explosionPlayed) { explosion_sound.play(); explosionPlayed = true; }
        Pos p = player.GetCurrentPos();
        expl.animExplosion(p.x - 120, p.y - 43);
    }
    if (alive == 0)
    {
        gameOver = true; gameOverText.show();
        records.tryUpdate(score);
        return;
    }

    player.AnimJet();
    player.shipControl();
    player.moveShip(dt);
    indicator.animFuelIndicator(score);

    if (trashManager.update(dt, player.getBounds()))
    {
        if (alive > 0) { alive--; health[alive].destroy(); }
        if (alive == 0) { gameOver = true; gameOverText.show(); return; }
        trashManager.reset();
    }

    if (fuelManager.update(dt, player.getBounds()))
        indicator.refueling();

    if (indicator.getFrame() == 0 && alive > 0)
    {
        alive--; health[alive].destroy(); indicator.reset();
    }

    float bgSpeed = 150.0f;
    gamingBackground.move(0, bgSpeed * dt);
    pos = gamingBackground.getPosition();
    if (pos.y >= HEIGHT) gamingBackground.setPosition(pos.x, -HEIGHT);

    gamingBackground2.move(0, bgSpeed * dt);
    pos = gamingBackground2.getPosition();
    if (pos.y >= HEIGHT) gamingBackground2.setPosition(pos.x, -HEIGHT);

    Pos temp = player.GetCurrentPos();
    float dist = static_cast<float>(way.WayBetweenTwoPoint(temp.x, temp.y));
    score += static_cast<int>(dist);
    score += static_cast<int>(150.f * dt);

    text.setString(std::to_string(score));
    sf::FloatRect a = text.getLocalBounds();
    text.setPosition(WIDTH - a.width - 285.f, HEIGHT - 69);
    if (score > 1000) { text.setCharacterSize(14);  text.setPosition(WIDTH - a.width - 282.f, HEIGHT - 69); }
    if (score > 10000) { text.setCharacterSize(13); text.setPosition(WIDTH - a.width - 278.f, HEIGHT - 69); }
    if (score > 100000) { text.setCharacterSize(11);text.setPosition(WIDTH - a.width - 278.f, HEIGHT - 69); }

    speed = static_cast<int>(way.speedMeter(dist, dt));
    text1.setString(std::to_string(speed));
    sf::FloatRect b = text1.getLocalBounds();
    text1.setPosition(WIDTH - b.width - 185.f, HEIGHT - 53);

    if (addLive.update(dt, player.getBounds()))
        if (alive > 0 && alive < 3) { health[alive].resetHearts(); ++alive; }

    if (enemManager.update(dt, player.getBounds(), temp.x, temp.y))
    {
        if (alive > 0) { alive--; health[alive].destroy(); }
        if (alive == 0) { gameOver = true; gameOverText.show(); return; }
    }

    text3.setString(std::to_string(enemManager.getEscapeCount()));
}

void Game::render()
{
    window.clear();
    window.draw(gamingBackground2);
    window.draw(gamingBackground);
    window.draw(GameInfoPanel);

    gameOver == false ? player.draw(window) : expl.draw(window);

    fuelManager.draw(window);
    addLive.draw(window);
    trashManager.draw(window);
    enemManager.draw(window);
    indicator.draw(window);

    if (paused) pauseMenu.draw(window);

    health[2].draw(window); health[1].draw(window); health[0].draw(window);

    window.draw(text); window.draw(text1); window.draw(text2); window.draw(text3);

    bestText.setString("BEST: " + std::to_string(records.getBest()));
    window.draw(bestText);

    if (gameOver) gameOverText.draw(window);

    window.display();
}