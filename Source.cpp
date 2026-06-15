#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <algorithm> 
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>

static constexpr float WIDTH =  368 * 1.4f;
static constexpr float HEIGHT = 654 * 1.4f;


struct Pos
{
	int x;
	int y;
};

class AlgorithmiCalculations
{
public:
	template<typename T> void FisherYatesShuffle(std::vector<T>& array)
	{
		for (size_t i = array.size() - 1; i > 0; i--)
		{
			std::uniform_int_distribution<size_t> distr(0, i);
			size_t j = distr(rng);
			std::swap(array[i], array[j]);
		}
	}

	template<typename T> int WayBetweenTwoPoint(T _A, T _B)
	{
		result = sqrt(((_A - currentX) * (_A - currentX)) + ((_B - currentY) * (_B - currentY)));
		currentX = _A;
		currentY = _B;

		return result;
	}
	
	float speedMeter(float S, float t)
	{
		return (t > 0.f) ? (S / t) : 0.f;
	}

	float mergeTwoUnrelatedRanges(float _1from, float _1to, float _2from, float _2to)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> range1(-_1from, _1to);
		std::uniform_real_distribution<float> range2(_2from, _2to);
		std::uniform_int_distribution<int> choose_range(0, 1);
		return choose_range(gen) == 0 ?range1(gen) : range2(gen);
	}
private:
	int result = 0;
	int currentX = 0;
	int currentY = 0;

	std::mt19937 rng{ std::random_device{}() };
};

class Player
{
public:
	Player() : sprite(sf::Vector2f(88, 200))
	{
		playerTexture.loadFromFile("IMG/SHIP/broomhilda.png");
		sprite.setTexture(&playerTexture);
		sprite.setPosition(184, 600);

		//anim
		herotexture.loadFromFile("IMG/SHIP/broomhilda_animation.png");
		herosprite.setTexture(herotexture);
	}

	void shipControl()
	{
		targetSpeed = sf::Vector2f(0.0f, 0.0f);
		float speed = 500.0f; // скорость корабля в пикселях в секунду

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			targetSpeed.y = -speed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			targetSpeed.y = speed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			targetSpeed.x = -speed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			targetSpeed.x = speed;
	}
	void moveShip(float dt)
	{
		//LERP
		float acceleration = 8.f; 
		currentSpeed.x += (targetSpeed.x - currentSpeed.x) * acceleration * dt;
		currentSpeed.y += (targetSpeed.y - currentSpeed.y) * acceleration * dt;
		sprite.move(currentSpeed * dt);

		sf::Vector2f pos = sprite.getPosition();
		if (pos.x > WIDTH - 90) sprite.setPosition(WIDTH - 90, pos.y);
		if (pos.x < 5) sprite.setPosition(5, pos.y);
		if (pos.y > HEIGHT - 355) sprite.setPosition(pos.x, HEIGHT - 355);
		if (pos.y < 5) sprite.setPosition(pos.x, 5);

		if ((pos.x > WIDTH - 90) && (pos.y < 5)) sprite.setPosition(WIDTH - 90, 5);
		if ((pos.x > WIDTH - 90) && (pos.y > HEIGHT - 355)) sprite.setPosition(WIDTH - 90, HEIGHT - 355);
		if ((pos.x < 5) && (pos.y > HEIGHT - 355)) sprite.setPosition(5, HEIGHT - 355);
		if ((pos.x < 5) && (pos.y < 5)) sprite.setPosition(5, 5);
	}

	Pos GetCurrentPos()
	{
		Pos result;
		sf::Vector2f pos = sprite.getPosition();

		result.x = pos.x;
		result.y = pos.y;

		return result;
	}
	void SetCurrentPosition()
	{
		sprite.setPosition(184, 600);
	}

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
		herosprite.setPosition(sf::Vector2f(sprite.getPosition().x+17, sprite.getPosition().y + 181));
	}
	sf::FloatRect getBounds() 
	{
		sf::FloatRect bounds = sprite.getGlobalBounds();
		float sx = bounds.width * 0.3f; // 30% с каждой стороны по X
		float sy = 0;
		return sf::FloatRect(
			bounds.left + sx,
			bounds.top + sy,
			bounds.width - sx * 2,
			bounds.height - sy * 0
		);
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

	static constexpr int arrNumY[3]{0,62, 120};
	int Frame = 3, stepanim = -1;

	sf::Clock clockAnim;
};

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
	void draw(sf::RenderWindow& window)
	{
		window.draw(sprite);
	}
private:
	sf::Sprite  sprite;
	sf::Texture texture;
	                                                             
	static constexpr int arrNumX[39] = {
	0, 341, 682, 1023, 1364, 1705, 2046, 2387, 2728, 3069,
	3410, 3751, 4092, 4433, 4774, 5115, 5456, 5797, 6138, 6479,
	6820, 7161, 7502, 7843, 8184, 8525, 8866, 9207, 9548, 9889,
	10230, 10571, 10912, 11253, 11594, 11935, 12276,12610,13020
	};
	int Frame = 0, stepanim = 1;
	sf::Clock clock;
};
// один метеорит
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
		respawn(); // сразу размещаем
	}

	void respawn()
	{
		// случайный х по ширине экрана
		float x = static_cast<float>(rand() % (int)WIDTH);
		// спавн чуть выше экрана
		float y = -180.f;
		SpaceObject.setPosition(x, y);

		//размер
		float scale = static_cast<float>(rand() % 7 + 5) / 10.f; // 0.2..0.6
		SpaceObject.setScale(scale, scale);

		// скорость падения
		speedY = static_cast<float>(rand() % 200 + 100); // 100..300 px/s

		// вращение
		rotationSpeed = static_cast<float>(rand() % 121 - 60); // -60..+60
	}

	void update(float dt)
	{
		SpaceObject.move(0.0f, speedY * dt); // только вниз
		SpaceObject.rotate(rotationSpeed * dt);

		// если летел за низ экрана - респавн наверху
		if (SpaceObject.getPosition().y > HEIGHT - 135)
			respawn();
	}

	void draw(sf::RenderWindow& window)
	{
		window.draw(SpaceObject);
	}

	bool collision(sf::FloatRect playerBounds) const
	{
		sf::FloatRect bounds = SpaceObject.getGlobalBounds();
		//сжатия хитбокса на 20%
		float sx = bounds.width * 0.2f;
		float sy = bounds.height * 0.2f;
		sf::FloatRect hitbox(bounds.left + sx, bounds.top + sy,bounds.width - sx * 2, bounds.height - sy * 2);
		return hitbox.intersects(playerBounds);
	}
private:
	std::vector<std::string> images{ "1.png", "2.png", "5.png","6.png", "8_1.png","8_2.png","8_3.png" };
	AlgorithmiCalculations   shuffle;

	sf::Sprite  SpaceObject;
	sf::Texture TextureObject;
	float       speedY = 150.f;
	float       rotationSpeed = 0.f;

};

// менеджер
class TrashManager
{
public:
	TrashManager()
	{
		// создаём N метеоритов сразу 
		meteors.resize(COUNT);
	}

	// возвращает true если было столкновения
	bool update(float dt, sf::FloatRect playerBounds)
	{
		bool hit = false;
		for (auto& m : meteors)
		{
			m.update(dt);
			if (m.collision(playerBounds))
				hit = true;
		}
		return hit;
	}

	void draw(sf::RenderWindow& window)
	{
		for (auto& m : meteors) m.draw(window);
	}

	void reset()
	{
		for (auto& m : meteors) m.respawn();
	}
private:
	static constexpr int COUNT = 3; // сколько метеоритов одновременно
	std::vector<SpaceTrash> meteors;
};

class Health
{
public:
	Health(int X, int Y, int sizeX, int sizeY)
	{
		TextureObject.loadFromFile("IMG/Panel/health.png");
		Hearts.setTexture(TextureObject);
		Hearts.setTextureRect(sf::IntRect(0, 0, sizeX, sizeY));
		Hearts.setPosition(X, Y);
		/*Hearts.setPosition(340, HEIGHT - 75);*/
	}

	void draw(sf::RenderWindow& window)
	{
		window.draw(Hearts);
	}
	void destroy()
	{
		Hearts.setColor(sf::Color(255, 255, 255, 0));
	}
	void resetHearts()
	{
		Hearts.setColor(sf::Color(255, 255, 255, 255));
	}
private:
	sf::Sprite  Hearts;
	sf::Texture TextureObject;
};

class ViewGameOverTitle
{
public:
	ViewGameOverTitle()
	{
		TextureObject.loadFromFile("IMG/GAMEOVER/gameover.png");
		title.setTexture(TextureObject);
		title.setTextureRect(sf::IntRect(0, 0, 292, 150));
		title.setPosition((WIDTH/2 - (292/2)), HEIGHT / 2 - 150);
	}
	void hide()
	{
		title.setColor(sf::Color(255, 255, 255, 0));
	}
	void show()
	{
		title.setColor(sf::Color(255, 255, 255, 255));
	}
	void draw(sf::RenderWindow& window)
	{
		window.draw(title);
	}
private:
	sf::Sprite  title;
	sf::Texture TextureObject;
};

//базовый класс для Fuel и AddictiveLive
class PickupItem
{
public:
	PickupItem(const std::string& texturePath, sf::IntRect texRect,
		int marginX, float yMultiplier, int speedMin, int speedMax)
		: marginX(marginX), yMultiplier(yMultiplier),
		speedMin(speedMin), speedMax(speedMax)
	{
		texture.loadFromFile(texturePath);
		sprite.setTexture(texture);
		sprite.setTextureRect(texRect);
		spawn();
	}

	void spawn()
	{
		float x = static_cast<float>(rand() % (int)(WIDTH - marginX - marginX + 1) + marginX);
		float y = -yMultiplier * static_cast<float>(rand() % (int)HEIGHT) - 180.f;
		sprite.setPosition(x, y);
		speedY = static_cast<float>(rand() % (speedMax - speedMin) + speedMin);
	}

	void update(float dt)
	{
		sprite.move(0.f, speedY * dt);
		if (sprite.getPosition().y > HEIGHT - 140)
			spawn();
	}

	void draw(sf::RenderWindow& window)
	{
		window.draw(sprite);
	}

	bool collision(sf::FloatRect playerBounds) const
	{
		return sprite.getGlobalBounds().intersects(playerBounds);
	}


protected:
	sf::Sprite  sprite;
	sf::Texture texture;
	float       speedY = 0.f;

private:
	int   marginX;
	float yMultiplier;
	int   speedMin;
	int   speedMax;
};

//fuel
class FuelIndicator
{
public:
	FuelIndicator()
	{
		TextureObject.loadFromFile("IMG/charge/fuelSpriteList2.png");
		indicator.setTexture(TextureObject);
		indicator.setPosition(452, 812);
		//indicator.setPosition();
	}
	void animFuelIndicator(int score)
	{
		if ((score % 750 == 0))
		{
			if (Frame > 0) Frame += stepanim;
		}
		indicator.setTextureRect(sf::IntRect(arrNumX[Frame], 0, 34, 60));
	}
	void refueling()
	{
		if(Frame < 8) Frame++;
	}
	int getFrame() const
	{
		return Frame;
	}
	void reset()
	{
		Frame = 8;
	}
	void draw(sf::RenderWindow& window)
	{
		window.draw(indicator);
	}
private:
	sf::Sprite  indicator;
	sf::Texture TextureObject;
	sf::Clock	clock;
	static constexpr int arrNumX[9] = {0,38,77,116,153,190,227,263,299};
	int Frame = 8, stepanim = -1;
};


class Fuel : public PickupItem
{
public:
	//                    текстура            rect              margin  yMult  speedMin  speedMax
	Fuel() : PickupItem("IMG/FUEL/fuel.png", sf::IntRect(0, 0, 38, 50), 38, 2.f, 100, 300) {}
};

class FuelManager
{
public:
	FuelManager()
	{
		objects.resize(COUNT);
	}
	bool update(float dt, sf::FloatRect playerBounds)
	{
		bool hit = false;
		for (auto& obj : objects)
		{
			obj.update(dt);
			if (obj.collision(playerBounds))
			{
				obj.spawn();
				hit = true;
			}
		}
		return hit;
	}
	void draw(sf::RenderWindow& window)
	{
		for (auto& obj : objects) obj.draw(window);
	}
	void reset()
	{
		for (auto& obj : objects) obj.spawn();
	}
private:
	static constexpr int COUNT = 3; 
	std::vector<Fuel> objects;
};

class Enemies
{
public:
	Enemies(float playerX, float playerY)
	{
		//ALG.FisherYatesShuffle(images);
		//TextureObject.loadFromFile("IMG/Enemies/" + images[0]);
		TextureObject.loadFromFile("IMG/Enemies/drone1.png");
		Enem.setTexture(TextureObject);
		Enem.setTextureRect(sf::IntRect(0, 0, 75, 97));
		spawn(playerX, playerY);
	}
	void hide()
	{
		Enem.setPosition(-1000.f, -1000.f);
		velocity = { 0.f, 0.f };
	}
	void spawn(float playerX, float playerY)
	{
		int min_valX = WIDTH - 150;
		int max_valX = WIDTH + 150;
		float x = ALG.mergeTwoUnrelatedRanges(min_valX, -51, WIDTH + 50, max_valX);
	
		int min_valY = 0;
		int max_valY = HEIGHT - 350;
		float y = static_cast<float>(min_valY + rand() % (max_valY - min_valY + 1)); //rand() % (max - min + 1) + min;
		Enem.setPosition(x, y);
		// направление к игроку
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
		if (Enem.getPosition().y > HEIGHT - 145 || Enem.getPosition().x > WIDTH + 160 || Enem.getPosition().x < -160)
		{
			spawn(playerX, playerY);
			return true;
		}
		return false;
	}
	void draw(sf::RenderWindow& window)
	{
		window.draw(Enem);
	}
	bool collision(sf::FloatRect playerBounds) const
	{
		sf::FloatRect bounds = Enem.getGlobalBounds();
		//сжатия хитбокса на 10%
		float sx = bounds.width * 0.1f;
		float sy = bounds.height * 0.1f;
		sf::FloatRect hitbox(bounds.left + sx, bounds.top + sy, bounds.width - sx * 2, bounds.height - sy * 2);
		return hitbox.intersects(playerBounds);
	}

private:
	AlgorithmiCalculations   ALG;
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
				for (auto& obj : objects)
					obj.spawn(playerX, playerY);
			}
			return false;
		}

		for (auto& obj : objects)
		{
			for (auto& obj : objects)
			{
				bool escaped = obj.update(dt, playerX, playerY);
				if (escaped)
				{
					totalEscaped++;
					continue; // не проверяем коллизию на только что заспавненном объекте
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
		}
		return hit;
	}
	void resetEscapeCount()
	{
		totalEscaped = 0;
	}
	int getEscapeCount() const 
	{ 
		return totalEscaped; 
	}
	void draw(sf::RenderWindow& window)
	{
		for (auto& obj : objects) obj.draw(window);
	}
	void reset(float playerX, float playerY)
	{
		for (auto& obj : objects) obj.spawn(playerX, playerY);
	}
private:
	std::vector<Enemies> objects;
	const INT COUNT = 1;

	sf::Clock respawnTimer;
	bool waiting = false;

	int totalEscaped = 0;

};

class AddictiveLive : public PickupItem
{
public:
	AddictiveLive() : PickupItem("IMG/heart/add_heart.png", sf::IntRect(0, 0, 59, 50), 60, 5.f, 160, 360) {}
};

class AddictiveLiveManager
{
public:
	AddictiveLiveManager()
	{
		objects.resize(COUNT);
	}
	bool update(float dt, sf::FloatRect playerBounds)
	{
		bool hit = false;
		for (auto& obj : objects)
		{
			obj.update(dt);
			if (obj.collision(playerBounds))
			{
				obj.spawn();
				hit = true;
			}
		}
		return hit;
	}
	void draw(sf::RenderWindow& window)
	{
		for (auto& obj : objects) obj.draw(window);
	}
	void reset()
	{
		for (auto& obj : objects) obj.spawn();
	}
private:
	static constexpr int COUNT = 1;
	std::vector<AddictiveLive> objects;
};

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
	enum class Action { None, Resume, Restart, Records, Exit };

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
		t.setPosition(WIDTH / 2.f - 100.f, y-50.f);
	}

	sf::RectangleShape overlay;
	sf::Text title;
	sf::Text btnResume;
	sf::Text btnRestart;
	sf::Text btnExit;
};

class Records
{
public:
	Records()
	{
		load();
	}

	void tryUpdate(int score)
	{
		if (score > bestScore)
		{
			bestScore = score;
			save();
		}
	}
	int getBest() 
	{ 
		return bestScore;
	}

private:
	void save()
	{
		std::ofstream file("Records/text.txt");
		if (file.is_open())
			file << bestScore;
	}
	void load()
	{
		std::ifstream file("Records/text.txt");
		if (file.is_open())
			file >> bestScore;
	}
	int bestScore = 0;
};


class Game
{
public:
	Game()
		: window(sf::VideoMode(WIDTH, HEIGHT), "Cosmo Battle!"),
		GameInfoPanel(sf::Vector2f(WIDTH, 136)),
		gamingBackground(sf::Vector2f(WIDTH, HEIGHT)),
		gamingBackground2(sf::Vector2f(WIDTH, HEIGHT)),
		dt(0.0)
	{
		//музыка
		music.openFromFile("Music/iron_tid_rising.ogg");
		music.setLoop(true);
		music.setVolume(5);
		music.play();

		explosion_sound.openFromFile("Music/0527.ogg");
		explosion_sound.setPitch(2.f);
		explosion_sound.setVolume(10);

		//icon
		window.setVerticalSyncEnabled(true);
		if (!icon.loadFromFile("IMG/icon.png"))
			throw std::runtime_error("cannot load icon");
		window.setIcon(32, 32, icon.getPixelsPtr());

		// Панель
		TexturInfoPanel.loadFromFile("IMG/Panel/PA_NEL2-Photoroom.png");
		GameInfoPanel.setTexture(&TexturInfoPanel);
		GameInfoPanel.setPosition(0, HEIGHT - 136);

		//background
		textureSpace.loadFromFile("IMG/background/space_back2.jpg");
		gamingBackground.setTexture(&textureSpace);
		gamingBackground2.setTexture(&textureSpace);
		gamingBackground2.setPosition(0, -HEIGHT);

		//text
		if (!font.loadFromFile("fonts/LCD16x2Remastered-RegularV2.otf"))
			throw std::runtime_error("cannot load font");

		//score
		text.setFont(font);
		text.setCharacterSize(15);
		text.setFillColor(sf::Color::White);
		text.setPosition(WIDTH - 360, HEIGHT - 67);

		//speed
		text1.setFont(font);
		text1.setCharacterSize(12);
		text1.setFillColor(sf::Color::White);
		//text1.setPosition(290, HEIGHT - 53);

		//KM/h
		text2.setFont(font);
		text2.setCharacterSize(10);
		text2.setFillColor(sf::Color::White);
		text2.setPosition(340, HEIGHT - 53);
		text2.setString("KM/h");

		//destr
		text3.setFont(font);
		text3.setCharacterSize(15);
		text3.setFillColor(sf::Color::White);
		text3.setPosition(83, HEIGHT - 71);
		//text3.setString("0");

		//record
		bestText.setFont(font);
		bestText.setCharacterSize(10);
		bestText.setFillColor(sf::Color::White);
		bestText.setPosition(20, HEIGHT - HEIGHT + 15);
	}
	void run()
	{
		while (window.isOpen())
		{
			updateTime();
			processEvents();
			update();
			render();
		}
	}
private:
	void updateTime()
	{
		dt = clock.restart().asSeconds();
	}
	void processEvents()
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			// рестарт по пробелу 
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space && gameOver && !paused)
			{
				gameOver = false;
				alive = 3;
				health[2].resetHearts();
				health[1].resetHearts();
				health[0].resetHearts();
				trashManager.reset();
				fuelManager.reset();
				addLive.reset();
				expl.reset();
				indicator.reset();
				Pos p = player.GetCurrentPos();
				enemManager.reset(p.x, p.y);
				enemManager.resetEscapeCount();
				player.SetCurrentPosition();
				explosionPlayed = false;
				explosion_sound.stop();
				//music.play();
				score = 0;
			}
			//пауза
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
			{
				if (gameOver)
					gameOverText.hide();
				paused = !paused;
				if (paused)
				{
					music.pause();
					explosion_sound.pause();
				}
				else
				{
					if (gameOver)
						gameOverText.show();
					music.play();
					if (!(explosion_sound.getStatus() == sf::Sound::Stopped))
					{
						explosion_sound.play();
					}
				}
			}

			if (paused && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
			{
				auto action = pauseMenu.handleClick(sf::Mouse::getPosition(window));
				//if (gameOver) gameOverText.hide();
				switch (action)
				{
				case PauseMenu::Action::Resume:
					paused = false;
					music.play();
					if (gameOver) gameOverText.show();
					break;

				case PauseMenu::Action::Restart:
					paused = false;
					gameOver = false;
					alive = 3;
					score = 0;
					health[2].resetHearts();
					health[1].resetHearts();
					health[0].resetHearts();
					trashManager.reset();
					fuelManager.reset();
					enemManager.resetEscapeCount();
					expl.reset();
					indicator.reset();
					explosionPlayed = false;
					explosion_sound.stop();
					player.SetCurrentPosition();
					music.play();
					break;

				case PauseMenu::Action::Exit:
					window.close();
					break;

				default: break;
				}
			}

			if (paused)
				pauseMenu.handleHover(sf::Mouse::getPosition(window));
		}
	}
	void update()
	{
		if (paused) return;

		if (gameOver)
		{
			if (!explosionPlayed)
			{
				explosion_sound.play();
				explosionPlayed = true;
			}
			Pos p = player.GetCurrentPos();
			expl.animExplosion(p.x - 120, p.y - 43);
		}
		if (alive == 0)
		{
			gameOver = true;
			gameOverText.show();
			records.tryUpdate(score); //запись рекорда
			return;
		}
		player.AnimJet();
		player.shipControl();
		player.moveShip(dt);
		indicator.animFuelIndicator(score);

		//реализация трех жизней
		if (trashManager.update(dt, player.getBounds()))
		{
			if (alive > 0)
			{
				alive--;
				health[alive].destroy();
			}
			if (alive == 0)
			{
				gameOver = true;
				gameOverText.show();
				return;
			}
			trashManager.reset();
		}
		//топливо
		if (fuelManager.update(dt, player.getBounds()))
		{
			//fuel.destroy();
			indicator.refueling();
		}
		if (indicator.getFrame() == 0 && alive > 0)
		{
			alive--;
			health[alive].destroy();
			indicator.reset();

		}
		float bgSpeed = 150.0f;

		gamingBackground.move(0, bgSpeed * dt);
		pos = gamingBackground.getPosition();
		if (pos.y >= HEIGHT)
			gamingBackground.setPosition(pos.x, -HEIGHT);

		gamingBackground2.move(0, bgSpeed * dt);
		pos = gamingBackground2.getPosition();
		if (pos.y >= HEIGHT)
			gamingBackground2.setPosition(pos.x, -HEIGHT);

		Pos temp = player.GetCurrentPos();

		//обсчет процденого пути
		float dist = static_cast<float>(way.WayBetweenTwoPoint(temp.x, temp.y));
		score += static_cast<int>(dist);
		score += static_cast<int>(150.f * dt); // 150 в сек
		text.setString(std::to_string(score));
		sf::FloatRect a = text.getLocalBounds();
		text.setPosition(WIDTH - a.width - 285.f, HEIGHT - 69); //прижать число к правому краю
		if (score > 1000) { text.setCharacterSize(14); text.setPosition(WIDTH - a.width - 282.f, HEIGHT - 69); }
		if (score > 10000) { text.setCharacterSize(13); text.setPosition(WIDTH - a.width - 278.f, HEIGHT - 69); }
		if (score > 100000) { text.setCharacterSize(11); text.setPosition(WIDTH - a.width - 278.f, HEIGHT - 69); }
		//text.setPosition(WIDTH - 360, HEIGHT - 67);

		//скорость
		speed = static_cast<int>(way.speedMeter(dist, dt));
		text1.setString(std::to_string(speed));
		sf::FloatRect b = text1.getLocalBounds();
		text1.setPosition(WIDTH - b.width - 185.f, HEIGHT - 53); //прижать число к правому краю

		//доп жизни
		if (addLive.update(dt, player.getBounds()))
		{
			if (alive > 0 && alive < 3)
			{
				health[alive].resetHearts();
				++alive;
			}
		}
		//enemies
		if (enemManager.update(dt, player.getBounds(), temp.x, temp.y))
		{
			if (alive > 0)
			{
				alive--;
				health[alive].destroy();
			}
			if (alive == 0)
			{
				gameOver = true;
				gameOverText.show();
				return;
			}
		}
		int countOfDesrEnem = enemManager.getEscapeCount();
		text3.setString(std::to_string(countOfDesrEnem));
	}
	void render()
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


		health[2].draw(window);
		health[1].draw(window);
		health[0].draw(window);

		window.draw(text); //score
		window.draw(text1); //speed
		window.draw(text2); //destr enem
		window.draw(text3); //text3

		bestText.setString("BEST: " + std::to_string(records.getBest()));
		window.draw(bestText);

		if (gameOver) gameOverText.draw(window); //gameOver

		window.display();
	}
private:
	sf::RenderWindow   window;
	sf::Image          icon;

	sf::Texture        TexturInfoPanel;
	sf::RectangleShape GameInfoPanel;

	sf::Texture        textureSpace;
	sf::RectangleShape gamingBackground;
	sf::RectangleShape gamingBackground2;

	sf::Vector2f pos;
	sf::Clock    clock;
	float        dt;

	Player player;

	sf::Font font;
	sf::Text text;  //score
	sf::Text text1; //speed
	sf::Text text2; //km/h
	sf::Text text3; //destroied

	int score = -594;
	int temp = 0;

	TrashManager  trashManager;
	FuelManager   fuelManager;
	FuelIndicator indicator;
	Fuel          fuel;

	AddictiveLiveManager addLive;

	EnemManager enemManager{ 184.f, 600.f }; // начальная позиция игрока

	bool gameOver = false;

	AlgorithmiCalculations way;

	Health health[3]
	{
		Health(291, HEIGHT - 121,33,16),
		Health(326, HEIGHT - 121,34,16),
		Health(361, HEIGHT - 121,35,16)
	};

	int alive = 3;
	int speed = 0;

	sf::Clock  timer;
	sf::Music  music;
	sf::Music  explosion_sound;
	ShipDamage expl;

	ViewGameOverTitle gameOverText;
	PauseMenu pauseMenu{ font };


	Records records;
	sf::Text bestText;

	bool explosionPlayed = false;
	bool paused = false;

};

int main()
{
	srand(time(NULL));
	Game game;
	game.run();
	return 0;
}