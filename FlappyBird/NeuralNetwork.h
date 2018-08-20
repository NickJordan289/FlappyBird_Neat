/*
	Credit to Daniel Shiffman's videos on Perceptrons, Matrix Math and Neural Networks
*/

#pragma once
#include "Matrix.h"
#include <vector>
#include "ExtraFuncs.h"

// function pointer type
// type name : doubleFunction
// param1 : double
// return : double
typedef double(*doubleFunction)(double);

class NeuralNetwork {
private:
	Matrix weights_ih, weights_ho, bias_h, bias_o;
	double trainingRate;
	doubleFunction activation;
	doubleFunction derivative;

public:
	inline static double sigmoid(double x) {
		return 1.0 / (1.0 + exp(-x));
	}

	inline static double sigmoid_d(double x) {
		return x * (1.0 - x);
	}

	NeuralNetwork(int inputNeurons, int hiddenLayerNeurons, int outputNeurons, double trainingRate=0.1, doubleFunction activation=sigmoid, doubleFunction derivative=sigmoid_d) {
		this->trainingRate = trainingRate;
		this->activation = activation;
		this->derivative = derivative;

		weights_ih = Matrix(hiddenLayerNeurons, inputNeurons, true);
		weights_ho = Matrix(outputNeurons, hiddenLayerNeurons, true);
		bias_h = Matrix(hiddenLayerNeurons, 1, true);
		bias_o = Matrix(outputNeurons, 1, true);
	}

	// Feed forward implementation
	Matrix predict(Matrix inputs) {

	// Feed Forward
		// calculate neuron values between input and hidden
		// hidden = activation((weights*input)+bias)
		Matrix hidden = Matrix::dot(weights_ih, inputs);
		hidden += bias_h;
		hidden.map(activation);

		// calculate neuron values between hidden and output
		// output = activation((weights*hidden)+bias)
		Matrix outputs = Matrix::dot(weights_ho, hidden);
		outputs += bias_o;
		outputs.map(activation);

		// Return the result as an array rather than a single column matrix
		return outputs;
	}
	
	void train(Matrix inputs, Matrix targets, bool debug=false) {
	
	// Feed Forward
		// calculate neuron values between input and hidden
		// hidden = activation((weights*input)+bias)
		Matrix hidden = Matrix::dot(weights_ih, inputs);
		hidden += bias_h;
		hidden.map(sigmoid);

		// calculate neuron values between hidden and output
		// output = activation((weights*hidden)+bias)
		Matrix outputs = Matrix::dot(weights_ho, hidden);
		outputs += bias_o;
		outputs.map(sigmoid);

	// Back Propagation
	// Hidden to Output Weights

		// error = how far off the output was
		// gradient = how much each weight influenced the output
		Matrix outputErrors = targets - outputs;
		Matrix outputGradients = Matrix::Map(outputs, sigmoid_d);
		outputGradients *= outputErrors;
		outputGradients *= trainingRate;

		// hidden->output deltas
		Matrix hoAdjustments = Matrix::dot(outputGradients, hidden.T());
		weights_ho += hoAdjustments;
		bias_o += outputGradients;

	// Input to Hidden Weights

		// error = how far off the output was
		// gradient = how much each weight influenced the output
		Matrix hiddenErrors = Matrix::dot(weights_ho.T(), outputErrors);
		Matrix hiddenGradients = Matrix::Map(hidden, sigmoid_d);
		hiddenGradients *= hiddenErrors;
		hiddenGradients *= trainingRate;

		// input->hidden deltas
		Matrix ihAdjustments = Matrix::dot(hiddenGradients, inputs.T());
		weights_ih += ihAdjustments;
		bias_h += hiddenGradients;
	}

	// helper function that converts input to matrix before guessing
	inline Matrix predict(std::vector<double> inputs) {
		return predict(Matrix::fromVector(inputs));
	}

	// helper function that converts parameters to matrices before training
	inline void train(std::vector<double> inputs, std::vector<double> targets, bool debug = false) {
		train(Matrix::fromVector(inputs), Matrix::fromVector(targets), debug);
	}

	// maps each weight and bias in place
	inline void mutate(doubleFunction func, double chance) {
		weights_ih.map(func, chance);
		weights_ho.map(func, chance);
		bias_h.map(func, chance);
		bias_o.map(func, chance);
	}

	// maps each weight and bias returning a copy
	static inline NeuralNetwork mutate(NeuralNetwork a, doubleFunction func, double chance) {
		NeuralNetwork temp = a;
		temp.mutate(func, chance);
		return temp;
	}
};