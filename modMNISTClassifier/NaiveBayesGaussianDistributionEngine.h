#pragma once

#define _USE_MATH_DEFINES

#include <vector>
#include <thread>
#include <chrono>
#include <array>
#include <algorithm>
#include <math.h>

#include "../modMNIST/MNISTLoader.h"

class NaiveBayesGaussianDistributionEngine
{
public:
	NaiveBayesGaussianDistributionEngine();
	~NaiveBayesGaussianDistributionEngine();

	void train(std::vector<unsigned char> imageVector, const size_t imageLabel);

	void trainFinalize();

	size_t classify(std::vector<unsigned char> imageVector);

private:

	const double startScore;

	std::vector<size_t> 
		pixelColorStatistics,
		classSizes;

	std::vector<double>
		mu,
		sigmaSquare;

};

