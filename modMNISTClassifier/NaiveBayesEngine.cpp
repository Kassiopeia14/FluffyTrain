#include "NaiveBayesEngine.h"

NaiveBayesEngine::NaiveBayesEngine():
	pixelColorStatistics(MNISTLoader::imageSize * MNISTLoader::colorScale)
{
}

NaiveBayesEngine::~NaiveBayesEngine()
{
}

size_t NaiveBayesEngine::classify(std::vector<unsigned char> imageVector)
{
	// P(c|d) ~ P(d|c) * P(c) = P(d[0]|c) * .. * P(d[m]|c) * P(c)
	// P(d[i]|c) = n(d in c && d[i] == imageVector[i]) / n(d in c)
	// ln(P(c|d)) ~ sum_by_i(ln(n(d in c && d[i] == imageVector[i]))) - 28 * 28 * ln(n(d in c))
	return 5;
}