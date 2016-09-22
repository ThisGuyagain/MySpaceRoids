// Engine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "GameObjects.h"
#include <string>
#include <vector>

bool fullscreen = false;
int main()
{
	srand(static_cast<unsigned int>(time(0)));
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Welcome to SDVA 103!");		
	Game game;
	
	sf::Clock clock;
	while (window.isOpen())
	{
		sf::Time dt = clock.restart();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Return &&
					event.key.alt)
				{					
					window.close();
					fullscreen = !fullscreen;
					window.create(fullscreen ? sf::VideoMode(1920,1080) : sf::VideoMode(1280, 720), "Welcome to SDVA 103!", fullscreen ? sf::Style::Fullscreen : sf::Style::Titlebar);
				}
				if (event.key.code == sf::Keyboard::G)
				{				
					game.RemoveAll();
				}
			}
			if (event.type == sf::Event::Closed)
				window.close();
		}
		
		window.clear();	
		game.Update(&window, dt.asSeconds());
		game.Draw(&window);
		window.display();
	}

	return 0;
}
