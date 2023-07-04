#pragma once

#include <vector>
#include <thread>
#include <chrono>

class RandomEngine
{
public:
	RandomEngine();
	~RandomEngine();

	void train(std::vector<unsigned char> imageVector, const size_t imageLabel);

	size_t classify(std::vector<unsigned char> imageVector);

private:

};
