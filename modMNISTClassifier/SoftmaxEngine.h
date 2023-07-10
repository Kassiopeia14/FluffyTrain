#pragma once

#include <vector>
#include <thread>
#include <chrono>
#include <array>
#include <algorithm>

#include "../modMNIST/MNISTLoader.h"

class SoftmaxEngine
{
public:
	SoftmaxEngine();
	~SoftmaxEngine();

	const char* getName();

	bool stopCondition(const size_t epoch);

	void train(std::vector<unsigned char> imageVector, const size_t imageLabel);

	void trainFinalize();

	size_t classify(std::vector<unsigned char> imageVector);

private:

	static const char* name;

	std::vector<double>
		weights,
		biases;

	const double startScore;

	std::vector<size_t>
		pixelColorStatistics,
		classSizes;

	static std::vector<double> initializeWeights();

	static std::vector<double> initializeBiases();
};
