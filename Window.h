#pragma once

#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>
#include <limits>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

class Game
{
private:
	//Variables
	//Window
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event ev;

	//Mouse positions
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	//Resources
	sf::Font font;

	//Text
	sf::Text uiText;
	sf::Text uiLoseGame;
	sf::Text uiPause;
	std::string difficult;

	//Game logic
	bool endGame;
	bool isGamePaused;
	bool mouseHeld;
	unsigned points;
	int health;
	int maxEnemies;
	float enemySpawnTimer;
	float enemySpawnTimerMax;

	//Game objects
	std::vector<sf::RectangleShape> enemies;
	sf::RectangleShape enemy;

	//Private functions
	void initVariables();
	void initWindow();
	void initFonts();
	void initText();
	void initEnemies();
	void changeDifficult();
	void restartGame();
public:
	//Constructors / Destructors
	Game();
	virtual ~Game();

	//Accessors
	const bool running() const;
	const bool getEndGame() const;

	//Functions
	void spawnEnemy();

	void pollEvents();
	void updateMousePositions();
	void updateText();
	void updateEnemies();
	void update();

	void renderText(sf::RenderTarget& target);
	void renderEnemies(sf::RenderTarget& target);
	void render();
};