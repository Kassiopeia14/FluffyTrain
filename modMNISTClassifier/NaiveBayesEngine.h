#pragma once

#include <vector>

#include "../modMNIST/MNISTLoader.h"

class NaiveBayesEngine
{
public:
	NaiveBayesEngine();

	~NaiveBayesEngine();

	size_t classify(std::vector<unsigned char> imageVector);

private:

	std::vector<size_t> pixelColorStatistics;

};

