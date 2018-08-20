#pragma once
#include <deque>
#include <memory>

#include <SFML/Graphics.hpp>

#include "NeuralNetwork.h"

#include "Pipe.h"
#include "ExtraFuncs.h"

class Bird {
public:
	sf::RectangleShape r;

	sf::Vector2f velocity = sf::Vector2f(0.f, 0.f);
	float minVelocity = -30.f;
	float maxVelocity = 30.f;

	bool crashed = false;
	bool shouldDraw = true;

	sf::RenderWindow& rWindRef;
	std::deque<std::shared_ptr<Pipe>>& pipesRef;
	int& lifetimeCounterRef;

	std::vector<double> vision = { 0.0, 0.0, 0.0, 0.0, 0.0 };
	NeuralNetwork nn = NeuralNetwork(vision.size(), 4, 1);
	double fitness = 0;

	Bird(NeuralNetwork nn, sf::RenderWindow& rWind, std::deque<std::shared_ptr<Pipe>>& pipes, int& lifetime) : Bird(rWind, pipes, lifetime) {
		this->nn = nn;
	}

	Bird(sf::RenderWindow& rWind, std::deque<std::shared_ptr<Pipe>>& pipes, int& lifetime) : rWindRef(rWind), pipesRef(pipes), lifetimeCounterRef(lifetime) {
		r.setSize(sf::Vector2f(32.f, 32.f));
		r.setOrigin(r.getGlobalBounds().width / 2.f, r.getGlobalBounds().height / 2.f);
		r.setPosition(sf::Vector2f(64.f, rWindRef.getSize().y / 2.f));
		r.setFillColor(sf::Color(255, 255, 255, 100));
		r.setOutlineColor(sf::Color(255, 255, 255, 200));
		r.setOutlineThickness(0.5f);
	}

	void jump() {
		velocity += sf::Vector2f(0.f, -15.f);
		velocity.y = constrain(velocity.y, minVelocity, maxVelocity);
	}

	// inputs
	// 0 : normalised y position
	// 1 : normalised y velocity
	// 2 : normalised height of top pipe
	// 3 : normalised height of bottom pipe
	// 4 : normalised distance to end of pipe
	void think() {
		vision[0] = map(r.getPosition().y, 0.0, rWindRef.getSize().y, 1.0, 0.0);
		vision[1] = map(velocity.y, minVelocity, maxVelocity, 0.f, 1.f);
		vision[2] = map(pipesRef[0]->top.getGlobalBounds().top+pipesRef[0]->top.getGlobalBounds().height, 0.0, rWindRef.getSize().y, 1.0, 0.0);
		vision[3] = map(pipesRef[0]->bot.getGlobalBounds().top, 0.0, rWindRef.getSize().y, 1.0, 0.0);
		
		double distance = (pipesRef[0]->top.getGlobalBounds().left + pipesRef[0]->top.getGlobalBounds().width) - r.getGlobalBounds().left;
		if (distance < 0.0)
			distance = (pipesRef[1]->top.getGlobalBounds().left + pipesRef[1]->top.getGlobalBounds().width) - r.getGlobalBounds().left;
		vision[4] = map(distance, 0.0, rWindRef.getSize().x - r.getGlobalBounds().left, 0.0, 1.0);

		std::vector<double> output = nn.predict(vision).map(round).toVector();
		if (output[0] == 1) {
			jump();
		}
	}

	void update(float speed) {

		think();

		if(!crashed) {
			velocity += sf::Vector2f(0.f, 0.6f); // gravity
			velocity *= 0.9f; // air resistance
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

			if (crashed) {
				fitness = lifetimeCounterRef;
				fitness = pow(fitness, 2);
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