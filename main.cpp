#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Game!", sf::Style::Titlebar | sf::Style::Close);
	sf::Event ev;

	while (window.isOpen())
	{
		while (window.pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.display();
	}
    return 0;
}