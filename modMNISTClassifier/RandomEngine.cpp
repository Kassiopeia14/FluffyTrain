#include "RandomEngine.h"

RandomEngine::RandomEngine()
{
}

RandomEngine::~RandomEngine()
{
}

void RandomEngine::train(std::vector<unsigned char> imageVector, const size_t imageLabel)
{
	std::this_thread::sleep_for(std::chrono::microseconds(1));
}

size_t RandomEngine::classify(std::vector<unsigned char> imageVector)
{
	std::this_thread::sleep_for(std::chrono::microseconds(2));

	return rand() % 10;
}