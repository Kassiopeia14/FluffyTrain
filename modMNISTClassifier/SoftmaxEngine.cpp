#include "SoftmaxEngine.h"

const char* SoftmaxEngine::name("Softmax");

SoftmaxEngine::SoftmaxEngine():
	theta(0.001),
	weights(initializeWeights()),
	biases(initializeBiases())
{
}

SoftmaxEngine::~SoftmaxEngine()
{
}

const char* SoftmaxEngine::getName()
{
	return name;
}

bool SoftmaxEngine::stopCondition(const size_t epoch)
{
	return epoch > 4;
}

std::vector<double> SoftmaxEngine::initializeWeights()
{
	std::vector<double> weightsValues(MNISTLoader::imageSize * MNISTLoader::classCount);

	for (auto weightValue = weightsValues.begin(); weightValue != weightsValues.end(); weightValue++)
	{
		*weightValue = (1. - ((double)(rand() % 2000)) / 1000) / 100;
	}

	return weightsValues;
}

std::vector<double> SoftmaxEngine::initializeBiases()
{
	std::vector<double> biasesValues(MNISTLoader::classCount);

	for (auto biaseValue = biasesValues.begin(); biaseValue != biasesValues.end(); biaseValue++)
	{
		*biaseValue = (1. - ((double)(rand() % 2000)) / 1000) / 100;
	}

	return biasesValues;
}

std::vector<double> SoftmaxEngine::softmax(std::array<double, MNISTLoader::classCount> z)
{
	std::vector<double> result(MNISTLoader::classCount, 0.);

	double zMax = 0.;
	size_t maxIndex = z.size();

	for (int i = 0; i < z.size(); i++)
	{
		if (z[i] > zMax)
		{
			zMax = z[i];
			maxIndex = i;
		}
	}

	if (zMax > 1000.)
	{
		result[maxIndex] = 1.;

		return result;
	}

	double sum = 0.;

	auto
		resultItem = result.begin();
	auto
		zItem = z.begin();

	for (; zItem != z.end(); zItem++, resultItem++)
	{
		const double e = exp(*zItem);

		*resultItem = e;

		sum += e;
	}

	for (resultItem = result.begin(); resultItem != result.end(); resultItem++)
	{
		*resultItem /= sum;
	}

	return result;
}

void SoftmaxEngine::train(std::vector<unsigned char> imageVector, const size_t imageLabel)
{
	//std::this_thread::sleep_for(std::chrono::microseconds(1));

	std::array<double, MNISTLoader::classCount> z;

	for (int k = 0; k < MNISTLoader::classCount; k++)
	{
		double sum = 0.;

		for (int i = 0; i < MNISTLoader::imageSize; i++)
		{
			sum += weights[MNISTLoader::imageSize * k + i] * imageVector[i] + biases[k];
		}

		z[k] = sum;
	}

	std::vector<double> db(softmax(z));

	db[imageLabel] -= 1.;

	for (int k = 0; k < MNISTLoader::classCount; k++)
	{
		biases[k] -= theta * db[k];

		for (int i = 0; i < MNISTLoader::imageSize; i++)
		{
			weights[MNISTLoader::imageSize * k + i] -= theta * db[k] * imageVector[i];
		}
	}
}

void SoftmaxEngine::trainFinalize()
{
}

size_t SoftmaxEngine::classify(std::vector<unsigned char> imageVector)
{
	//std::this_thread::sleep_for(std::chrono::microseconds(1));

	std::array<double, MNISTLoader::classCount> z;

	for (int k = 0; k < MNISTLoader::classCount; k++)
	{
		double sum = 0.;

		for (int i = 0; i < MNISTLoader::imageSize; i++)
		{
			sum += weights[MNISTLoader::imageSize * k + i] * imageVector[i] + biases[k];
		}

		z[k] = sum;
	}

	std::vector<double> p(softmax(z));

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