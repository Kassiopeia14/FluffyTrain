#pragma once

#include <vector>
#include <thread>
#include <chrono>

class RandomEngine
{
public:
	RandomEngine();
	~RandomEngine();

	const char* getName();

	bool stopCondition(const size_t epoch);

	void train(std::vector<unsigned char> imageVector, const size_t imageLabel);

	void trainFinalize();

	size_t classify(std::vector<unsigned char> imageVector);

private:

	static const char* name;

};
