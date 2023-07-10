#include "RandomEngine.h"

const char* RandomEngine::name("Random");

RandomEngine::RandomEngine()
{
}

RandomEngine::~RandomEngine()
{
}

const char* RandomEngine::getName()
{
	return name;
}

bool RandomEngine::stopCondition(const size_t epoch)
{
	return epoch > 0;
}

void RandomEngine::train(std::vector<unsigned char> imageVector, const size_t imageLabel)
{
	std::this_thread::sleep_for(std::chrono::microseconds(1));
}

void RandomEngine::trainFinalize()
{
	;
}

size_t RandomEngine::classify(std::vector<unsigned char> imageVector)
{
	std::this_thread::sleep_for(std::chrono::microseconds(2));

	return rand() % 10;
}