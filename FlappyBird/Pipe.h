#pragma once
#include <random>

#include <SFML/Graphics.hpp>

#include "ExtraFuncs.h"

class Pipe{
public:
	sf::RectangleShape top, bot;
	sf::RenderWindow& rWindRef;

	Pipe(sf::RenderWindow& rWind) : rWindRef(rWind) {
		float randomPosition = randomDouble(96, rWindRef.getSize().y-96);
		float randomSize = randomDouble(112, 160);

		top.setSize(sf::Vector2f(48.f, (randomPosition - randomSize / 2)));
		top.setPosition(rWindRef.getSize().x, 0.f);

		bot.setSize(sf::Vector2f(48.f, -(rWindRef.getSize().y - randomPosition - randomSize / 2)));
		bot.setPosition(rWindRef.getSize().x, rWindRef.getSize().y);
	}

	void update(float speed) {
		top.move(sf::Vector2f(-speed, 0));
		bot.move(sf::Vector2f(-speed, 0));
	}

	void draw() {
		rWindRef.draw(top);
		rWindRef.draw(bot);
	}
};