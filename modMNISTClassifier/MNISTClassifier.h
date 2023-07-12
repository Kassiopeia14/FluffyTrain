#pragma once

#include <vector>
#include <atomic>
#include <thread>

#include"../modMNIST/MNISTLoader.h"

enum ClassifierStage
{
	train,
	test
};

struct ClassifierState
{
	ClassifierStage stage;

	std::vector<unsigned char> image;

	size_t 
		realLabel,
		predictedLabel,
		number;

	bool 
		classified,
		tested;

	double successRate;
};

template<class Engine>
class MNISTClassifier
{
private:

	Engine& engine;

	MNISTLoader& mnistLoader;

	std::atomic_flag lock;
	std::atomic<bool>& running;

	ClassifierStage currentStage;

	std::vector<unsigned char> currentImage;

	size_t
		currentResult,
		currentImageNumber,
		currentRealLabel;

	double currentSuccessRate;

	size_t batchSize;

	bool 
		classified,
		tested;

	std::thread workThread;

	void train();

	void test();

public:
	MNISTClassifier(
		Engine& _engine,
		MNISTLoader& _mnistLoader,
		std::atomic<bool>& _running);

	~MNISTClassifier();

	void operator()();

	std::string getEngineName() const;

	ClassifierState getCurrentState();

	std::vector<double> getClassPixelDistribution(
		size_t classLabel,
		size_t x, 
		size_t y);
};

template<class Engine>
MNISTClassifier<Engine>::MNISTClassifier(
	Engine& _engine,
	MNISTLoader& _mnistLoader,
	std::atomic<bool>& _running):
	engine(_engine),
	mnistLoader(_mnistLoader),
	lock(),
	running(_running),
	batchSize(60000),
	currentImage(MNISTLoader::imageSize),
	tested(false),
	workThread(std::ref(*this))
{
}

template<class Engine>
MNISTClassifier<Engine>::~MNISTClassifier()
{
	workThread.join();
}

template<class Engine>
void MNISTClassifier<Engine>::operator()()
{
	size_t epoch = 0;

	while (running.load() && !engine.stopCondition(epoch))
	{
		train();
		engine.trainEpochFinalize();

		test();

		epoch++;
	}

	engine.trainFinalize();

}

template<class Engine>
void MNISTClassifier<Engine>::train()
{
	const size_t trainImageCount(mnistLoader.getTrainImagesCount());

	while (lock.test_and_set())
	{
		std::this_thread::yield();
	}

	currentStage = ClassifierStage::train;

	classified = false;

	lock.clear();

	size_t
		imageNumber = 0;

	while (running.load() && (imageNumber < trainImageCount))
	{
		std::vector<unsigned char> image = mnistLoader.getTrainImage(imageNumber);

		const size_t
			realLabel = mnistLoader.getTrainLabel(imageNumber);

		engine.train(image, realLabel);

		if ((imageNumber % batchSize == batchSize - 1) || (imageNumber == trainImageCount - 1))
		{
			engine.batchTrainFinalize();
		}

		if (!lock.test_and_set())
		{
			memcpy(&currentImage[0], &image[0], image.size());

			currentRealLabel = realLabel;

			currentImageNumber = imageNumber;

			classified = true;

			lock.clear();
		}

		imageNumber++;
	}
}

template<class Engine>
void MNISTClassifier<Engine>::test()
{
	const size_t testImageCount(mnistLoader.getTestImagesCount());

	//const size_t testImageCount(mnistLoader.getTrainImagesCount());

	size_t 
		hitCount = 0,
		imageNumber = 0;

	while (lock.test_and_set())
	{
		std::this_thread::yield();
	}

	currentStage = ClassifierStage::test;

	classified = false;

	lock.clear();

	while (running.load() && (imageNumber < testImageCount))
	{
		std::vector<unsigned char> image = mnistLoader.getTestImage(imageNumber);
		
		//std::vector<unsigned char> image = mnistLoader.getTrainImage(imageNumber);

		const size_t
			realLabel = mnistLoader.getTestLabel(imageNumber),
			//realLabel = mnistLoader.getTrainLabel(imageNumber),
			result = engine.classify(image);

		if (realLabel == result)
		{
			hitCount++;
		}
		if (!lock.test_and_set())
		{
			memcpy(&currentImage[0], &image[0], image.size());

			currentResult = result;

			currentRealLabel = realLabel;

			currentImageNumber = imageNumber;

			if (imageNumber == testImageCount - 1)
			{
				currentSuccessRate = (double)hitCount / (imageNumber + 1);

				tested = true;
			}
			classified = true;

			lock.clear();
		}

		imageNumber++;
	}
}

template<class Engine>
std::string MNISTClassifier<Engine>::getEngineName() const
{
	return engine.getName();
}

template<class Engine>
ClassifierState MNISTClassifier<Engine>::getCurrentState()
{
	while (lock.test_and_set())
	{
		std::this_thread::yield();
	}

	ClassifierState classifierState
	{
		.stage = currentStage,
		.image = std::vector<unsigned char>(currentImage.size()),
		.realLabel = currentRealLabel,
		.predictedLabel = currentResult,
		.number = currentImageNumber,
		.classified = classified,
		.tested = tested,
		.successRate = currentSuccessRate
	};

	memcpy(&classifierState.image[0], &currentImage[0], currentImage.size());

	lock.clear();

	return classifierState;
}

template<class Engine>
std::vector<double> MNISTClassifier<Engine>::getClassPixelDistribution(
	size_t classLabel,
	size_t x,
	size_t y)
{
	return engine.getClassPixelDistribution(classLabel, x, y);
}