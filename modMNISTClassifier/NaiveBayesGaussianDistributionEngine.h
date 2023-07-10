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

	const char* getName();

	bool stopCondition(const size_t epoch);

	void train(std::vector<unsigned char> imageVector, const size_t imageLabel);

	void trainFinalize();

	size_t classify(std::vector<unsigned char> imageVector);

	std::vector<double> getClassPixelDistribution(
		size_t classLabel,
		size_t x,
		size_t y);

private:

	static const char* name;

	const double startScore;

	std::vector<size_t> 
		pixelColorStatistics,
		classSizes;

	std::vector<double>
		mu,
		sigmaSquare;
};

