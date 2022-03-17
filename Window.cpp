#include "Window.h"

//Private functions
void Game::initVariables()
{
	this->window				= nullptr;

	//Game logic
	this->endGame				= false;
	this->points				= 0;
	this->health				= 1;
	this->enemySpawnTimerMax	= 80.f;
	this->enemySpawnTimer		= this->enemySpawnTimerMax;
	this->maxEnemies			= 5;
	this->mouseHeld				= false;
	this->difficult				= "Very Easy";
}

void Game::restartGame()
{
	this->endGame = false;
	this->points = 0;
	this->health = 1;
	this->enemySpawnTimerMax = 80.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->maxEnemies = 5;
	this->mouseHeld = false;
	this->difficult = "Very Easy";
	this->enemies.clear();
}

void Game::initWindow()
{
	this->videoMode.height = 600;
	this->videoMode.width = 800;

	this->window = new sf::RenderWindow(this->videoMode, "Hook It!", sf::Style::Titlebar | sf::Style::Close);

	this->window->setFramerateLimit(60);
}

void Game::initFonts()
{
	if (this->font.loadFromFile("./src/fonts/Fredoka-Light.ttf"))
	{
		std::cout << "ERROR::GAME::INITFONTS::Failed to load font!" << "\n";
	}
}

void Game::initText()
{
	this->uiText.setFont(this->font);
	this->uiText.setCharacterSize(24);
	this->uiText.setFillColor(sf::Color::White);
	this->uiText.setString("NONE");

	this->uiLoseGame.setFont(this->font);
	this->uiLoseGame.setCharacterSize(36);
	this->uiLoseGame.setFillColor(sf::Color::Red);
	this->uiLoseGame.setPosition(sf::Vector2f(this->window->getSize().x / 2 - 286 / 2, this->window->getSize().y / 2 - 70 / 2));
	this->uiLoseGame.setString("    GAME OVER\nPress R to restart");
}

void Game::initEnemies()
{
	this->enemy.setPosition(10.f, 10.f);
	this->enemy.setSize(sf::Vector2f(100.f, 100.f));
	this->enemy.setFillColor(sf::Color::Cyan);
}

void Game::changeDifficult()
{
	if (this->points < 100 && this->points >= 0) 
	{
		this->enemySpawnTimerMax = 60.f;
		this->maxEnemies = 6;
		this->difficult = "Very Easy";
		std::cout << "Difficult: Very Easy!" << std::endl;
	}
	else if (this->points < 200 && this->points >= 100)
	{
		this->enemySpawnTimerMax = 50.f;
		this->maxEnemies = 7;
		this->difficult = "Easy";
		std::cout << "Difficult: Easy!" << std::endl;
	}
	else if (this->points < 300 && this->points >= 200)
	{
		this->enemySpawnTimerMax = 40.f;
		this->maxEnemies = 8;
		this->difficult = "Medium";
		std::cout << "Difficult: Medium!" << std::endl;
	}
	else if (this->points < 400 && this->points >= 300)
	{
		this->enemySpawnTimerMax = 30.f;
		this->maxEnemies = 9;
		this->difficult = "Hard";
		std::cout << "Difficult: Hard!" << std::endl;
	}
	else //if (this->points < std::numeric_limits<int>::max() && this->points > 400)
	{
		this->enemySpawnTimerMax = 20.f;
		this->maxEnemies = 10;
		this->difficult = "Very Hard";
		std::cout << "Difficult: Very Hard!" << std::endl;
	}
}

//Constructors / Destructors
Game::Game()
{
	this->initVariables();
	this->initWindow();
	this->initFonts();
	this->initText();
	this->initEnemies();
}

Game::~Game()
{
	delete this->window;
}

//Accessors
const bool Game::running() const
{
	return this->window->isOpen();
}

const bool Game::getEndGame() const
{
	return this->endGame;
}

//Functions
void Game::spawnEnemy()
{
	this->enemy.setPosition(
		static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
		0.f
	);

	int type = rand() % 5;

	switch (type)
	{
	case 0:
		this->enemy.setSize(sf::Vector2f(30.f, 30.f));
		this->enemy.setFillColor(sf::Color::Magenta);
		break;
	case 1:
		this->enemy.setSize(sf::Vector2f(45.f, 45.f));
		this->enemy.setFillColor(sf::Color::Blue);
		break;
	case 2:
		this->enemy.setSize(sf::Vector2f(50.f, 50.f));
		this->enemy.setFillColor(sf::Color::Cyan);
		break;
	case 3:
		this->enemy.setSize(sf::Vector2f(65.f, 70.f));
		this->enemy.setFillColor(sf::Color::Red);
		break;
	case 4:
		this->enemy.setSize(sf::Vector2f(80.f, 80.f));
		this->enemy.setFillColor(sf::Color::Green);
		break;
	default:
		this->enemy.setSize(sf::Vector2f(100.f, 100.f));
		this->enemy.setFillColor(sf::Color::Yellow);
		break;
	}

	//Spawn the enemy
	this->enemies.push_back(this->enemy);
}

void Game::pollEvents()
{
	//Event polling
	while (this->window->pollEvent(this->ev))
	{
		switch (this->ev.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if (this->ev.key.code == sf::Keyboard::Escape)
				this->window->close();
			if (this->endGame && this->ev.key.code == sf::Keyboard::R)
				this->restartGame();
			break;
		}
	}
}

void Game::updateMousePositions()
{
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Game::updateText()
{
	std::stringstream ss;

	ss << "Points: " << this->points << "\n"
		<< "Health: " << this->health << "\n"
		<< "Difficult: " << this->difficult << "\n";

	this->uiText.setString(ss.str());
}

void Game::updateEnemies()
{
	if (!this->getEndGame())
	{
		if (window->hasFocus()) {
			//Updating the timer for enemy spawning
			if (this->enemies.size() < this->maxEnemies)
			{
				if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
				{
					//Spawn the enemy and reset the timer
					this->spawnEnemy();
					this->enemySpawnTimer = 0.f;
				}
				else
					this->enemySpawnTimer += 1.f;
			}

			//Moving and updating enemies
			for (int i = 0; i < this->enemies.size(); i++)
			{
				bool deleted = false;

				this->enemies[i].move(0.f, 5.f);

				if (this->enemies[i].getPosition().y > this->window->getSize().y)
				{
					this->enemies.erase(this->enemies.begin() + i);
					this->health -= 1;
					std::cout << "Health: " << this->health << "\n";
				}
			}

			//Check if clicked upon
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (this->mouseHeld == false)
				{
					this->mouseHeld = true;
					bool deleted = false;
					for (size_t i = 0; i < this->enemies.size() && deleted == false; i++)
					{
						if (this->enemies[i].getGlobalBounds().contains(this->mousePosView))
						{
							//Gain points
							if (this->enemies[i].getFillColor() == sf::Color::Magenta)
								this->points += 10;
							else if (this->enemies[i].getFillColor() == sf::Color::Blue)
								this->points += 7;
							else if (this->enemies[i].getFillColor() == sf::Color::Cyan)
								this->points += 5;
							else if (this->enemies[i].getFillColor() == sf::Color::Red)
								this->points += 3;
							else if (this->enemies[i].getFillColor() == sf::Color::Green)
								this->points += 1;

							this->changeDifficult();
							std::cout << "Points: " << this->points << "\n";

							//Delete the enemy
							deleted = true;
							this->enemies.erase(this->enemies.begin() + i);
						}
					}
				}
			}
			else
			{
				this->mouseHeld = false;
			}
		}
	}
}

void Game::update()
{
	this->pollEvents();

	if (this->endGame == false)
	{
		this->updateMousePositions();

		this->updateText();

		this->updateEnemies();
	}

	//End game condition
	if (this->health <= 0)
		this->endGame = true;
}

void Game::renderText(sf::RenderTarget& target)
{
	target.draw(this->uiText);

	if (this->getEndGame())
		target.draw(this->uiLoseGame);
}

void Game::renderEnemies(sf::RenderTarget& target)
{
	//Rendering all the enemies
	for (auto& e : this->enemies)
	{
		target.draw(e);
	}
}

void Game::render()
{
	this->window->clear();

	//Draw game objects
	this->renderEnemies(*this->window);

	this->renderText(*this->window);

	this->window->display();
}