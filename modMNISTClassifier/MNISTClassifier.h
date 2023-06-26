#pragma once

#include <vector>
#include <atomic>
#include <thread>

#include"../modMNIST/MNISTLoader.h"

struct ClassifiedImage
{
	std::vector<unsigned char> image;

	size_t label;
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

	size_t currentResult;

	std::thread workThread;

public:
	MNISTClassifier(
		Engine& _engine,
		MNISTLoader& _mnistLoader,
		std::atomic<bool>& _running);

	~MNISTClassifier();

	void operator()();

	ClassifiedImage getCurrentImage();
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
void MNISTClassifier<Engine>::operator()()
{
	size_t imageNumber = 0;

	while (running.load() && (imageNumber < 10000))
	{
		std::vector<unsigned char> image = mnistLoader.getImage(imageNumber);

		size_t result = engine.classify(image);

		if (!lock.test_and_set())
		{
			memcpy(&currentImage[0], &image[0], image.size());

			currentResult = result;

			lock.clear();
		}

		imageNumber++;
	}
}

template<class Engine>
MNISTClassifier<Engine>::~MNISTClassifier()
{
	workThread.join();
}

template<class Engine>
ClassifiedImage MNISTClassifier<Engine>::getCurrentImage()
{
	while (lock.test_and_set())
	{
		std::this_thread::yield();
	}

	ClassifiedImage classifiedImage
	{
		.image = std::vector<unsigned char>(currentImage.size()),
		.label = currentResult
	};

	memcpy(&classifiedImage.image[0], &currentImage[0], currentImage.size());

	lock.clear();

	return classifiedImage;
}