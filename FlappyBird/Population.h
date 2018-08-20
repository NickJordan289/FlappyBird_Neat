#pragma once
#include <vector>
#include <deque>
#include <memory>

#include <SFML/Graphics.hpp>

#include "Bird.h"
#include "Pipe.h"
#include "ExtraFuncs.h"

#include "NeuralNetwork.h"

class Population {
private:
	std::vector<std::shared_ptr<Bird>> agents;
	std::vector<std::shared_ptr<Bird>> matingPool;

	sf::RenderWindow& rWindRef;
	std::deque<std::shared_ptr<Pipe>>& pipesRef;
	int& lifetimeCounterRef;
	float speedRef;
	int popSize;
	float mutationChance;

public:
	Population(int popSize, float mutationChance, sf::RenderWindow& rWind, std::deque<std::shared_ptr<Pipe>>& pipes, int& lifetime, float& speed) : rWindRef(rWind), pipesRef(pipes), lifetimeCounterRef(lifetime), speedRef(speed) {
		this->popSize = popSize;
		this->mutationChance = mutationChance;
		for (int i = 0; i < popSize; i++) {
			agents.push_back(std::make_shared<Bird>(rWind, pipes, lifetime));
		}
	}

	// Fairly consistent across GA implementations
	// normalises the fitness and generates a mating pool
	void naturalSelection() {
		double highestFitness = 0.0;
		double sum = 0.0;
		for (std::shared_ptr<Bird> b : agents) {
			sum += b->fitness;
			if (b->fitness > highestFitness) {
				highestFitness = b->fitness;
			}
		}

		// normalise
		for (std::shared_ptr<Bird> b : agents) {
			b->fitness /= sum;
		}

		matingPool.clear();
		// add normalised agents to mating pool based on their fitness
		for (std::shared_ptr<Bird> b : agents) {
			int n = b->fitness * 100;
			for (int i = 0; i < n; i++) {
				matingPool.push_back(b);
			}
		}
		//std::cout << highestFitness << std::endl;
	}

	// Fairly consistent across GA implementations
	// generates the new population from the mating pool
	void selection() {
		std::vector<std::shared_ptr<Bird>> newPopulation;
		for (int i = 0; i < popSize; i++) {
			NeuralNetwork parentA = randomItem(matingPool)->nn;
			//NeuralNetwork parentB = randomItem(matingPool)->nn;
			//NeuralNetwork child = parentA.crossover(parentB);
			NeuralNetwork child = parentA; // placeholder till some crossover function is implemented

			child.mutate(shift, mutationChance);

			newPopulation.push_back(std::make_shared<Bird>(child, rWindRef, pipesRef, lifetimeCounterRef));
		}
		agents = newPopulation;
	}

	inline static double shift(double x) {
		return x + randomDouble(-0.1, 0.1); // push the weight between -0.1 or +0.1
	}

	void update() {
		// Move pipes and cull offscreen pipes
		for (auto p : pipesRef) {
			p->update(speedRef);
			if (p->top.getGlobalBounds().left + p->top.getGlobalBounds().width < 0.f) {
				pipesRef.pop_front();
			}
		}

		// Update our birds
		for (std::shared_ptr<Bird> b : agents) {
			b->update(speedRef);
		}
	}

	void draw() {
		for (std::shared_ptr<Pipe> p : pipesRef) {
			p->draw();
		}

		for (std::shared_ptr<Bird> b : agents) {
			b->draw();
		}
	}

	bool done() {
		for (std::shared_ptr<Bird> b : agents) {
			if (!b->crashed) {
				return false;
			}
		}
		return true;
	}

	void reset() {
		pipesRef.clear();
		pipesRef.push_back(std::make_shared<Pipe>(rWindRef));
		lifetimeCounterRef = 0;
	}
};