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

	void train(std::vector<unsigned char> imageVector, const size_t imageLabel);

	void trainFinalize();

	size_t classify(std::vector<unsigned char> imageVector);

private:

	const double startScore;

	std::vector<size_t>
		pixelColorStatistics,
		classSizes;

};
