#include "SoftmaxEngine.h"

const char* SoftmaxEngine::name("Softmax");

SoftmaxEngine::SoftmaxEngine():
	theta(.1),
	temperature(1.),
	wNorm(0.),
	cValue(std::numeric_limits<double>::max()),
	dWNorm(0.),
	weights(initializeWeights()),
	epochWeights(weights.begin(), weights.end()),
	newWeights(weights.begin(), weights.end()),
	biases(initializeBiases()),
	newBiases(biases.begin(), biases.end())
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
	return cValue < 1.e-05;
}

std::vector<double> SoftmaxEngine::initializeWeights()
{
	std::vector<double> weightsValues(MNISTLoader::imageSize * MNISTLoader::classCount);

	for (auto weightValue = weightsValues.begin(); weightValue != weightsValues.end(); weightValue++)
	{
		*weightValue = 0.;// (1. - ((double)(rand() % 2000)) / 1000) / 1000000;
	}

	return weightsValues;
}

std::vector<double> SoftmaxEngine::initializeBiases()
{
	std::vector<double> biasesValues(MNISTLoader::classCount);

	for (auto biaseValue = biasesValues.begin(); biaseValue != biasesValues.end(); biaseValue++)
	{
		*biaseValue = 0.;// (1. - ((double)(rand() % 2000)) / 1000) / 1000000;
	}

	return biasesValues;
}

std::vector<double> SoftmaxEngine::softmax(const double temperature, std::array<double, MNISTLoader::classCount> z)
{
	std::vector<double> result(MNISTLoader::classCount, 0.);

	double zMax = 0.;

	for (auto zItem = z.begin(); zItem != z.end(); zItem++)
	{
		if (*zItem > zMax)
		{
			zMax = *zItem;
		}
	}

	for (auto zItem = z.begin(); zItem != z.end(); zItem++)
	{
		*zItem -= zMax;
	}

	double sum = 0.;

	auto
		resultItem = result.begin();
	auto
		zItem = z.begin();

	for (; zItem != z.end(); zItem++, resultItem++)
	{
		const double e = exp(*zItem * temperature);

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

	std::vector<double> db(softmax(temperature, z));

	db[imageLabel] -= 1.;

	double sum2 = 0.;
	for (auto dbItem = db.begin(); dbItem != db.end(); dbItem++)
	{
		sum2 += *dbItem * *dbItem;
	}

	if (sum2 > 0.)
	{
		for (int k = 0; k < MNISTLoader::classCount; k++)
		{
			newBiases[k] -= theta * temperature * db[k];

			for (int i = 0; i < MNISTLoader::imageSize; i++)
			{
				const double dw = theta * temperature * db[k] * imageVector[i];
				newWeights[MNISTLoader::imageSize * k + i] -= dw;
			}
		}
	}
}

void SoftmaxEngine::trainEpochFinalize()
{
	//temperature *= 0.9;

	auto epochWeightItem = epochWeights.begin();
	for (auto weightItem = weights.begin(); weightItem != weights.end(); weightItem++, epochWeightItem++)
	{
		const double dw = *epochWeightItem - *weightItem;

		if (abs(dw) > dWNorm)
		{
			dWNorm = abs(dw);
		}

		*epochWeightItem = *weightItem;
	}

	double maxW = 0.;

	for (auto wItem = weights.begin(); wItem != weights.end(); wItem++)
	{
		if (abs(*wItem) > maxW)
		{
			maxW = *wItem;
		}
	}

	wNorm = maxW;

	cValue = dWNorm / wNorm;

	theta = cValue;

	if (theta > 0.1)
	{
		theta = 0.1;
	}

	dWNorm = 0.;
}

void SoftmaxEngine::batchTrainFinalize()
{
	auto newBiaseItem = newBiases.begin();
	for (auto biaseItem = biases.begin(); biaseItem != biases.end(); biaseItem++, newBiaseItem++)
	{
		*biaseItem = *newBiaseItem;
	}

	auto newWeightItem = newWeights.begin();
	for (auto weightItem = weights.begin(); weightItem != weights.end(); weightItem++, newWeightItem++)
	{
		*weightItem = *newWeightItem;
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

	std::vector<double> p(softmax(temperature, z));

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