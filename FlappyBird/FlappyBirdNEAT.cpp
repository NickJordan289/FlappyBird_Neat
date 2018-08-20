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
#include "Population.h"

int main() {
	srand(static_cast<unsigned int>(time(NULL)));

	const unsigned int SCREEN_WIDTH = 400;
	const unsigned int SCREEN_HEIGHT = 600;
	const unsigned int FPS = 0; //The desired FPS. (The number of updates each second) or 0 for uncapped.

	sf::Font ArialFont;
	ArialFont.loadFromFile("C:/Windows/Fonts/arial.ttf");

	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;

	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), ("Flappy Bird Clone " + GetEnvironmentBit()), sf::Style::Default, settings);
	window.setFramerateLimit(FPS);
	window.setKeyRepeatEnabled(false);
	
	std::deque<std::shared_ptr<Pipe>> pipes;
	pipes.push_back(std::make_shared<Pipe>(window));
	int lifetime = 0;
	int genCount = 1;
	float speed = 1.f; // doesn't work
	int popSize = 100;
	float weightMutationChance = 0.1f;
	
	Population pop = Population(popSize, weightMutationChance, window, pipes, lifetime, speed);
	std::cout << "Press 1 for a viewable speed\npress 2 for fast training" << std::endl;

#pragma region Graphics
	sf::Text lifetimeCounter;
	lifetimeCounter.setFont(ArialFont);
	lifetimeCounter.setString("0000");
	lifetimeCounter.setPosition(2, -4);
	lifetimeCounter.setOutlineColor(sf::Color::Black);
	lifetimeCounter.setOutlineThickness(1.f);

	sf::Text generationCounter;
	generationCounter.setFont(ArialFont);
	generationCounter.setString("1");
	generationCounter.setPosition(2, generationCounter.getCharacterSize()-4);
	generationCounter.setOutlineColor(sf::Color::Black);
	generationCounter.setOutlineThickness(1.f);

	sf::Text fpsCounter;
	fpsCounter.setFont(ArialFont);
	fpsCounter.setCharacterSize(20);
	fpsCounter.setString("0000");
	fpsCounter.setPosition(window.getSize().x - fpsCounter.getGlobalBounds().width-6, 0);
	fpsCounter.setOutlineColor(sf::Color::Black);
	fpsCounter.setOutlineThickness(1.f);
#pragma endregion

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
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
				{
					window.setFramerateLimit(60); // unlimited framerate for faster generations
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
				{
					window.setFramerateLimit(0); // unlimited framerate for faster generations
				}
			}
		}
		
		// Update
		if (!paused) {
			if (!pop.done()) {
				lifetime++;
				lifetimeCounter.setString(std::to_string(lifetime));

				if (lifetime % 200 == 0) {
					pipes.push_back(std::make_shared<Pipe>(window));
				}

				pop.update();
			} else {
				pop.naturalSelection();
				pop.selection();
				pop.reset();

				genCount++;
				generationCounter.setString(std::to_string(genCount));
			}
		}

		// Draw
		window.clear();

		pop.draw();

		window.draw(fpsCounter);
		window.draw(generationCounter);
		window.draw(lifetimeCounter);

		window.display();
	}

	return 0;
}