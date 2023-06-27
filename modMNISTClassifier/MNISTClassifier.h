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

	bool classified;

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

	bool classified;

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

	ClassifierState getCurrentState();
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
	currentImage(MNISTLoader::imageSize),
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
	train();
	test();
}

template<class Engine>
void MNISTClassifier<Engine>::train()
{
	const size_t trainImageCount(mnistLoader.getTrainImagesCount());

	size_t
		hitCount = 0,
		imageNumber = 0;

	while (lock.test_and_set())
	{
		std::this_thread::yield();
	}

	currentStage = ClassifierStage::train;

	classified = false;

	lock.clear();

	while (running.load() && (imageNumber < trainImageCount))
	{
		std::vector<unsigned char> image = mnistLoader.getTrainImage(imageNumber);

		const size_t 
			realLabel = mnistLoader.getTrainLabel(imageNumber),
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

			currentSuccessRate = (double)hitCount / (imageNumber + 1);

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

		const size_t
			realLabel = mnistLoader.getTestLabel(imageNumber),
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

			currentSuccessRate = (double)hitCount / (imageNumber + 1);

			classified = true;

			lock.clear();
		}

		imageNumber++;
	}
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
		.successRate = currentSuccessRate
	};

	memcpy(&classifierState.image[0], &currentImage[0], currentImage.size());

	lock.clear();

	return classifierState;
}