#pragma once

#include <windows.h>
#include "Painter.h"
#include "../modMNISTClassifier/MNISTClassifier.h"
#include "../modMNISTClassifier/RandomEngine.h"
#include "../modMNISTClassifier/NaiveBayesEngine.h"
#include "../modMNISTClassifier/NaiveBayesGaussianDistributionEngine.h"
#include "../modMNIST/MNISTLoader.h"

class MessageHandler
{
private:

	Painter painter;

	MNISTClassifier<NaiveBayesGaussianDistributionEngine>& mnistClassifier;

	std::atomic<bool>& running;

public:
	MessageHandler(
		MNISTClassifier<NaiveBayesGaussianDistributionEngine>& _mnistClassifier,
		std::atomic<bool>& _running);

	~MessageHandler();

	void onPaint(HDC deviceContext);
	
	void onClose();

};