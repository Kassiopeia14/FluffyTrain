#include "NaiveBayesEngine.h"

const char* NaiveBayesEngine::name("Naive Bayes");

NaiveBayesEngine::NaiveBayesEngine():
	startScore(0.06),
	pixelColorStatistics(MNISTLoader::imageSize * MNISTLoader::colorScale * MNISTLoader::classCount, 0),
	classSizes(MNISTLoader::classCount)
{
}

NaiveBayesEngine::~NaiveBayesEngine()
{
}

const char* NaiveBayesEngine::getName()
{
	return name;
}

bool NaiveBayesEngine::stopCondition(const size_t epoch)
{
	return epoch > 0;
}

void NaiveBayesEngine::train(std::vector<unsigned char> imageVector, const size_t imageLabel)
{
	//std::this_thread::sleep_for(std::chrono::microseconds(1));

	for (int i = 0; i < MNISTLoader::imageSize; i++)
	{
		//if (imageVector[i] > 0)
		//{
			const size_t statisticsIndex = imageLabel * MNISTLoader::imageSize * MNISTLoader::colorScale + i * MNISTLoader::colorScale + imageVector[i];

			pixelColorStatistics[statisticsIndex]++;
		//}
	}

	classSizes[imageLabel]++;
}

void NaiveBayesEngine::trainFinalize()
{

}

size_t NaiveBayesEngine::classify(std::vector<unsigned char> imageVector)
{
	//std::this_thread::sleep_for(std::chrono::seconds(1));

	// P(c|d) ~ P(d|c) * P(c) = P(d[0]|c) * .. * P(d[m]|c) * P(c)
	// P(d[i]|c) = n(d in c && d[i] == imageVector[i]) / n(d in c)
	// ln(P(c|d)) ~ sum_by_i(ln(n(d in c && d[i] == imageVector[i]))) - 28 * 28 * ln(n(d in c)) + ln(n(c))

	std::array<double, MNISTLoader::classCount> p;

	std::transform(classSizes.begin(), classSizes.end(), p.begin(), 
		[](const size_t classSize) 
		{
			return log(classSize);
		});

	for (int i = 0; i < MNISTLoader::imageSize; i++)
	{
		for (int k = 0; k < MNISTLoader::classCount; k++)
		{
			//if (imageVector[i] > 0)
			//{
				double score = startScore;

				const size_t statisticsIndex = k * MNISTLoader::imageSize * MNISTLoader::colorScale + i * MNISTLoader::colorScale + imageVector[i];

				score += pixelColorStatistics[statisticsIndex];

				p[k] += log(score / classSizes[k]);
			//}
		}
	}

	double maxP = log(0.);
	
	size_t classLabel = 10;

	for (int k = 0; k < MNISTLoader::classCount; k++)
	{
		if (p[k] > maxP)
		{
			maxP = p[k];
			classLabel = k;
		}
	}

	return classLabel;
}

std::vector<double> NaiveBayesEngine::getClassPixelDistribution(
	size_t classLabel,
	size_t x,
	size_t y)
{
	std::vector<double> result(256);

	double sum = 0.;

	int n = 0;

	for (int i = 0; i < 256; i++)
	{
		const size_t
			pixelIndex = y * MNISTLoader::imageSide + x,
			statisticsIndex = classLabel * MNISTLoader::imageSize * MNISTLoader::colorScale + pixelIndex * MNISTLoader::colorScale + i;

		result[i] = pixelColorStatistics[statisticsIndex];

		sum += result[i];

		int p = 32;

		if (i % p == p - 1)
		{
			sum /= p;

			for (int j = 0; j < p; j++)
			{
				result[n * p + j] = sum;
			}

			n++;
		}
	}

	//result[0] = 0;

	return result;
}