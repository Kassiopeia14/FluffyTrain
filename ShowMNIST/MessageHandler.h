#pragma once

#include <windows.h>
#include "Painter.h"
#include "../modMNISTClassifier/MNISTClassifier.h"
#include "../modMNISTClassifier/RandomEngine.h"
#include "../modMNISTClassifier/NaiveBayesEngine.h"
#include "../modMNISTClassifier/NaiveBayesGaussianDistributionEngine.h"
#include "../modMNISTClassifier/NaiveBayesHistogramEngine.h"
#include "../modMNISTClassifier/SoftmaxEngine.h"
#include "../modMNIST/MNISTLoader.h"

class MessageHandler
{
private:

	Painter painter;

	MNISTClassifier<SoftmaxEngine>& mnistClassifier;

	std::atomic<bool>& running;

	bool trainCompleted;

	std::vector<double> plotValues;

	double maxPlotValue;

public:
	MessageHandler(
		MNISTClassifier<SoftmaxEngine>& _mnistClassifier,
		std::atomic<bool>& _running);

	~MessageHandler();

	void setPlotValues(std::vector<double> _plotValues);

	void onPaint(HDC deviceContext);
	
	void onClose();

};