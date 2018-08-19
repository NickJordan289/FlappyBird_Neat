#pragma once
#include <deque>
#include <memory>

#include <SFML/Graphics.hpp>

#include "Pipe.h"
#include "ExtraFuncs.h"

class Bird {
public:
	sf::RectangleShape r;

	sf::Vector2f velocity = sf::Vector2f(0.f, 0.f);
	sf::Vector2f acc = sf::Vector2f(0.f, 0.f);

	bool crashed = false;
	bool shouldDraw = true;

	sf::RenderWindow& rWindRef;
	std::deque<std::shared_ptr<Pipe>>& pipesRef;

	Bird(sf::RenderWindow& rWind, std::deque<std::shared_ptr<Pipe>>& pipes) : rWindRef(rWind), pipesRef(pipes) {
		r.setSize(sf::Vector2f(32.f, 32.f));
		r.setOrigin(r.getGlobalBounds().width / 2.f, r.getGlobalBounds().height / 2.f);
		r.setPosition(sf::Vector2f(64.f, rWindRef.getSize().y / 2.f));
		r.setFillColor(sf::Color(255, 255, 255, 100));
		r.setOutlineColor(sf::Color::Black);
		r.setOutlineThickness(0.5f);
	}

	void jump() {
		velocity += sf::Vector2f(0.f, -15.f);
	}

	void update(float speed) {
		if(!crashed) {
			velocity += sf::Vector2f(0.f, 0.6f);
			velocity *= 0.9f;
			r.move(velocity);

			for (auto p : pipesRef) {
				if (r.getGlobalBounds().intersects(p->top.getGlobalBounds()) || r.getGlobalBounds().intersects(p->bot.getGlobalBounds())) {
					crashed = true;
				}
			}	

			if (r.getPosition().x > rWindRef.getSize().x || r.getPosition().x < 0) {
				crashed = true;
			}
			if (r.getPosition().y > rWindRef.getSize().y || r.getPosition().y < 0) {
				crashed = true;
			}
		} else {
			if (shouldDraw) {
				r.move(sf::Vector2f(-speed, 0)); // stick and move with the wall we crashed into
				if (r.getPosition().x < -r.getGlobalBounds().width) { // if offscreen
					shouldDraw = false;
				}
			}
		}

	}

	void draw() {
		if (shouldDraw) {
			rWindRef.draw(r);
		}
	}
};