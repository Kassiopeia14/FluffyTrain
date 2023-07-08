#include "SoftmaxEngine.h"

SoftmaxEngine::SoftmaxEngine():
	startScore(0.06),
	pixelColorStatistics(MNISTLoader::imageSize* MNISTLoader::colorScale* MNISTLoader::classCount, 0),
	classSizes(MNISTLoader::classCount)
{
}

SoftmaxEngine::~SoftmaxEngine()
{
}

void SoftmaxEngine::train(std::vector<unsigned char> imageVector, const size_t imageLabel)
{
	//std::this_thread::sleep_for(std::chrono::microseconds(1));

	for (int i = 0; i < MNISTLoader::imageSize; i++)
	{
		const size_t statisticsIndex = imageLabel * MNISTLoader::imageSize * MNISTLoader::colorScale + i * MNISTLoader::colorScale + imageVector[i];

		pixelColorStatistics[statisticsIndex]++;
	}

	classSizes[imageLabel]++;
}

void SoftmaxEngine::trainFinalize()
{
}

size_t SoftmaxEngine::classify(std::vector<unsigned char> imageVector)
{
	//std::this_thread::sleep_for(std::chrono::microseconds(1));

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
			double score = startScore;

			const size_t statisticsIndex = k * MNISTLoader::imageSize * MNISTLoader::colorScale + i * MNISTLoader::colorScale + imageVector[i];

			score += pixelColorStatistics[statisticsIndex];

			p[k] += log(score / classSizes[k]);
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