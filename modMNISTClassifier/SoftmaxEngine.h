#pragma once

#include <vector>
#include <thread>
#include <chrono>
#include <array>
#include <algorithm>
#include <limits>

#include "../modMNIST/MNISTLoader.h"

class SoftmaxEngine
{
public:
	SoftmaxEngine();
	~SoftmaxEngine();

	const char* getName();

	bool stopCondition(const size_t epoch);

	void train(std::vector<unsigned char> imageVector, const size_t imageLabel);

	void trainEpochFinalize();

	void trainFinalize();

	size_t classify(std::vector<unsigned char> imageVector);

private:

	static const char* name;

	double
		theta,
		temperature,
		wNorm,
		cValue,
		dWNorm;

	std::vector<double>
		weights,
		biases;

	static std::vector<double> initializeWeights();

	static std::vector<double> initializeBiases();
	
	static std::vector<double> softmax(const double temperature, std::array<double, MNISTLoader::classCount> z);
};
