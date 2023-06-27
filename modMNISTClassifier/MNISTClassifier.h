#pragma once

#include <vector>
#include <atomic>
#include <thread>

#include"../modMNIST/MNISTLoader.h"

struct ClassifierState
{
	std::vector<unsigned char> image;

	size_t 
		realLabel,
		predictedLabel,
		number;

	bool classified;
};

template<class Engine>
class MNISTClassifier
{
private:

	Engine& engine;

	MNISTLoader& mnistLoader;

	std::atomic_flag lock;
	std::atomic<bool>& running;

	std::vector<unsigned char> currentImage;

	size_t
		currentResult,
		currentImageNumber,
		currentRealLabel,
		currentHitCount;

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
	currentImageNumber = 0;
	classified = false;

	while (running.load() && (currentImageNumber < trainImageCount))
	{
		std::vector<unsigned char> image = mnistLoader.getTrainImage(currentImageNumber);

		size_t result = engine.classify(image);

		if (!lock.test_and_set())
		{
			memcpy(&currentImage[0], &image[0], image.size());

			currentResult = result;

			currentRealLabel = mnistLoader.getTrainLabel(currentImageNumber);

			classified = true;

			lock.clear();
		}

		currentImageNumber++;
	}
}

template<class Engine>
void MNISTClassifier<Engine>::test()
{
	const size_t testImageCount(mnistLoader.getTestImagesCount());
	currentImageNumber = 0;
	classified = false;

	while (running.load() && (currentImageNumber < testImageCount))
	{
		std::vector<unsigned char> image = mnistLoader.getTestImage(currentImageNumber);

		size_t result = engine.classify(image);

		if (!lock.test_and_set())
		{
			memcpy(&currentImage[0], &image[0], image.size());

			currentResult = result;

			currentRealLabel = mnistLoader.getTestLabel(currentImageNumber);

			classified = true;

			lock.clear();
		}

		currentImageNumber++;
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
		.image = std::vector<unsigned char>(currentImage.size()),
		.realLabel = currentRealLabel,
		.predictedLabel = currentResult,
		.number = currentImageNumber,
		.classified = classified
	};

	memcpy(&classifierState.image[0], &currentImage[0], currentImage.size());

	lock.clear();

	return classifierState;
}