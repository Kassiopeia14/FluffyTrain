#pragma once

#include <vector>
#include <thread>
#include <chrono>
#include <array>
#include <algorithm>

#include "../modMNIST/MNISTLoader.h"

class NaiveBayesHistogramEngine
{
public:
	NaiveBayesHistogramEngine();
	~NaiveBayesHistogramEngine();

	void train(std::vector<unsigned char> imageVector, const size_t imageLabel);

	void trainFinalize();

	size_t classify(std::vector<unsigned char> imageVector);

	std::vector<double> getClassPixelDistribution(
		size_t classLabel,
		size_t x,
		size_t y);

private:

	const double startScore;

	std::vector<size_t>
		pixelColorStatistics,
		pixelColorHistogramStatistics,
		classSizes;

};

