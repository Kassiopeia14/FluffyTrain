#include "RandomEngine.h"

RandomEngine::RandomEngine()
{
}

RandomEngine::~RandomEngine()
{
}

size_t RandomEngine::classify(std::vector<unsigned char> imageVector)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	return rand() % 10;
}