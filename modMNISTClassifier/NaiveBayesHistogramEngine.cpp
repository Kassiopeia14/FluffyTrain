#include "NaiveBayesHistogramEngine.h"

NaiveBayesHistogramEngine::NaiveBayesHistogramEngine():
	startScore(0.06),
	pixelColorStatistics(MNISTLoader::imageSize * MNISTLoader::colorScale * MNISTLoader::classCount, 0),
	pixelColorHistogramStatistics(MNISTLoader::imageSize* MNISTLoader::colorScale* MNISTLoader::classCount, 0),
	classSizes(MNISTLoader::classCount)
{
}

NaiveBayesHistogramEngine::~NaiveBayesHistogramEngine()
{
}

void NaiveBayesHistogramEngine::train(std::vector<unsigned char> imageVector, const size_t imageLabel)
{
	//std::this_thread::sleep_for(std::chrono::microseconds(1));

	for (int i = 0; i < MNISTLoader::imageSize; i++)
	{
		const size_t statisticsIndex = imageLabel * MNISTLoader::imageSize * MNISTLoader::colorScale + i * MNISTLoader::colorScale + imageVector[i];

		pixelColorStatistics[statisticsIndex]++;
	}

	classSizes[imageLabel]++;
}

void NaiveBayesHistogramEngine::trainFinalize()
{
	int windowSize = 4;

	for (int i = 0; i < MNISTLoader::imageSize; i++)
	{
		for (int k = 0; k < MNISTLoader::classCount; k++)
		{
			for (int j = 0; j < MNISTLoader::colorScale / windowSize; j++)
			{
				const int histogramStatisticsIndex = k * MNISTLoader::imageSize * MNISTLoader::colorScale + i * MNISTLoader::colorScale + j;

				size_t sum = 0;

				for (int d = 0; d < windowSize; d++)
				{
					const int statisticsIndex = k * MNISTLoader::imageSize * MNISTLoader::colorScale + i * MNISTLoader::colorScale + j * windowSize + d;
				
					sum += pixelColorStatistics[statisticsIndex];
				}
				
				for (int d = 0; d < windowSize; d++)
				{
					const int statisticsIndex = k * MNISTLoader::imageSize * MNISTLoader::colorScale + i * MNISTLoader::colorScale + j * windowSize + d;

					pixelColorHistogramStatistics[statisticsIndex] = sum / windowSize;
				}
			}
		}
	}
}

size_t NaiveBayesHistogramEngine::classify(std::vector<unsigned char> imageVector)
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
			double score = startScore;

			const size_t statisticsIndex = k * MNISTLoader::imageSize * MNISTLoader::colorScale + i * MNISTLoader::colorScale + imageVector[i];

			score += pixelColorHistogramStatistics[statisticsIndex]; // pixelColorStatistics[statisticsIndex];

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

std::vector<double> NaiveBayesHistogramEngine::getClassPixelDistribution(
	size_t classLabel,
	size_t x,
	size_t y)
{
	const int pixelIndex = y * MNISTLoader::imageSide + x;

	std::vector<double> result(256);

	for (int i = 0; i < 256; i++)
	{
		const int statisticsIndex = classLabel * MNISTLoader::imageSize * MNISTLoader::colorScale + pixelIndex * MNISTLoader::colorScale + i;

		result[i] = pixelColorHistogramStatistics[statisticsIndex];
	}

	//result[0] = 0;

	return result;
}