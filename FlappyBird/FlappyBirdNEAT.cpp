#include <iostream>
#include <vector>
#include <time.h>
#include <string>
#include <memory>
#include <deque>

#include <SFML/Graphics.hpp>

#include "Bird.h"
#include "Pipe.h"
#include "ExtraFuncs.h"

int main() {
	srand(static_cast<unsigned int>(time(NULL)));

	const unsigned int SCREEN_WIDTH = 400;
	const unsigned int SCREEN_HEIGHT = 600;
	const unsigned int FPS = 60; //The desired FPS. (The number of updates each second) or 0 for uncapped.

	sf::Font ArialFont;
	ArialFont.loadFromFile("C:/Windows/Fonts/arial.ttf");

	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;

	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), ("Flappy Bird Clone " + GetEnvironmentBit()), sf::Style::Default, settings);
	window.setFramerateLimit(FPS);
	window.setKeyRepeatEnabled(false);
	
	std::deque<std::shared_ptr<Pipe>> pipes;
	pipes.push_back(std::make_shared<Pipe>(window));
	std::shared_ptr<Bird> b = std::make_shared<Bird>(window, pipes);
	int lifetime = 0;
	float speed = 1.f;
	
	sf::Text lifetimeCounter;
	lifetimeCounter.setFont(ArialFont);
	lifetimeCounter.setString("0000");
	lifetimeCounter.setPosition(0, 0);
	lifetimeCounter.setOutlineColor(sf::Color::Black);
	lifetimeCounter.setOutlineThickness(1.f);

	sf::Text fpsCounter;
	fpsCounter.setFont(ArialFont);
	fpsCounter.setCharacterSize(20);
	fpsCounter.setString("000");
	fpsCounter.setPosition(window.getSize().x - fpsCounter.getGlobalBounds().width, 0);
	fpsCounter.setOutlineColor(sf::Color::Black);
	fpsCounter.setOutlineThickness(1.f);

	bool paused = false;
	sf::Event ev;
	sf::Clock clock;
	while (window.isOpen()) {
		float deltaTime = clock.restart().asSeconds();
		float fps = 1.f / deltaTime;
		fpsCounter.setString(std::to_string((int)fps));

		// Events
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed)
				window.close();

			if (ev.type == sf::Event::KeyPressed) {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
					window.close();
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
					paused = !paused;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
					b->jump();
				}
			}
		}
		
		// Update
		if (!paused) {
			if (b->crashed) {
				std::cout << "Score: " << lifetime << std::endl;
				pipes.clear();
				pipes.push_back(std::make_shared<Pipe>(window));
				b = std::make_shared<Bird>(window, pipes); // Replace bird with new one
				lifetime = 0;
			} 

			lifetime++;
			lifetimeCounter.setString(std::to_string(lifetime));

			if (lifetime % 200 == 0) {
				pipes.push_back(std::make_shared<Pipe>(window));
			}

			// Move pipes and cull offscreen pipes
			for (auto p : pipes) {
				p->update(speed);
				if (p->top.getGlobalBounds().left + p->top.getGlobalBounds().width < 0.f) {
					pipes.pop_front();
				}
			}

			// Update our bird
			b->update(speed);
		}

		// Draw
		window.clear();

		for (auto p : pipes) {
			p->draw();
		}

		b->draw();

		window.draw(fpsCounter);
		window.draw(lifetimeCounter);

		window.display();
	}

	return 0;
}