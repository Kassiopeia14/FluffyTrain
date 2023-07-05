#include "NaiveBayesGaussianDistributionEngine.h"

NaiveBayesGaussianDistributionEngine::NaiveBayesGaussianDistributionEngine() :
	startScore(0.06),
	pixelColorStatistics(MNISTLoader::imageSize * MNISTLoader::colorScale * MNISTLoader::classCount, 0),
	classSizes(MNISTLoader::classCount),
	mu(MNISTLoader::imageSize * MNISTLoader::classCount, 0),
	sigmaSquare(MNISTLoader::imageSize * MNISTLoader::classCount, 0)
{
}

NaiveBayesGaussianDistributionEngine::~NaiveBayesGaussianDistributionEngine()
{
}

void NaiveBayesGaussianDistributionEngine::train(std::vector<unsigned char> imageVector, const size_t imageLabel)
{
	//std::this_thread::sleep_for(std::chrono::microseconds(1));

	for (int i = 0; i < MNISTLoader::imageSize; i++)
	{
		const size_t statisticsIndex = imageLabel * MNISTLoader::imageSize * MNISTLoader::colorScale + i * MNISTLoader::colorScale + imageVector[i];

		pixelColorStatistics[statisticsIndex]++;
	}

	classSizes[imageLabel]++;
}

void NaiveBayesGaussianDistributionEngine::trainFinalize()
{
	//std::this_thread::sleep_for(std::chrono::microseconds(1));

	// mu = 1/n * sum(x_i)
	// sigma^2 = 1/(n-1) * sum((x_i - mu)^2)

	for (int k = 0; k < MNISTLoader::classCount; k++)
	{
		for (int i = 0; i < MNISTLoader::imageSize; i++)
		{
			double muSum = 0.;

			for (int j = 0; j < MNISTLoader::colorScale; j++)
			{
				const size_t statisticsIndex = k * MNISTLoader::imageSize * MNISTLoader::colorScale + i * MNISTLoader::colorScale + j;

				muSum += pixelColorStatistics[statisticsIndex];
			}

			const size_t muIndex = k * MNISTLoader::imageSize + i;

			mu[muIndex] = muSum / MNISTLoader::colorScale;
			
			double sigmaSquareSum = 0.;

			for (int j = 0; j < MNISTLoader::colorScale; j++)
			{
				const size_t statisticsIndex = k * MNISTLoader::imageSize * MNISTLoader::colorScale + i * MNISTLoader::colorScale + j;

				sigmaSquareSum += (pixelColorStatistics[statisticsIndex] - mu[muIndex]) * (pixelColorStatistics[statisticsIndex] - mu[muIndex]);
			}

			sigmaSquare[muIndex] = sigmaSquareSum / (MNISTLoader::colorScale - 1);
		}
	}
}

size_t NaiveBayesGaussianDistributionEngine::classify(std::vector<unsigned char> imageVector)
{
	//std::this_thread::sleep_for(std::chrono::seconds(1));

	//f(x) = (1 / sqrt(2 * M_PI * sigmaSquare)) * exp(-(x - mu) * (x - mu) / 2 * sigmaSquare);

	// P(c|d) ~ P(d|c) * P(c) = P(d[0]|c) * .. * P(d[m]|c) * P(c)
	// P(d[i]|c) = n(d in c && d[i] == imageVector[i]) / n(d in c) = (1 / sqrt(2 * pi * sigmaSquare)) * exp(-(imageVector[i] - mu)^2 / 2 * sigmaSquare)
	// ln(P(c|d)) ~ sum_by_i(-0.5 * ln(2 * pi * sigmaSquare) - (imageVector[i] - mu)^2 / 2 * sigmaSquare) + ln(n(c))

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
			double score = 0.;

			const size_t muIndex = k * MNISTLoader::imageSize + i;

			const double 
				muItem = mu[muIndex],
				sigmaSquareItem = sigmaSquare[muIndex];

			p[k] += - 0.5 * log(2 * M_PI * sigmaSquareItem) - (imageVector[i] - muItem) * (imageVector[i] - muItem) / 2 * sigmaSquareItem;
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