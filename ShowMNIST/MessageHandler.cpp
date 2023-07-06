#include "MessageHandler.h"

MessageHandler::MessageHandler(
	MNISTClassifier<NaiveBayesHistogramEngine>& _mnistClassifier,
	std::atomic<bool>& _running):
	painter(MNISTLoader::imageSide),
	mnistClassifier(_mnistClassifier),
	trainCompleted(false),
	plotValues(256, 0.),
	maxPlotValue(1.),
	running(_running)
{
}

MessageHandler::~MessageHandler()
{
}

void MessageHandler::onPaint(HDC deviceContext)
{
	ClassifierState currentState(mnistClassifier.getCurrentState());

	if (currentState.classified)
	{
		painter.paintImage(deviceContext, currentState.image);

		painter.beginText();

		std::string stageText("stage: " + (currentState.stage == ClassifierStage::train ? std::string("TRAIN") : std::string("TEST")));
		painter.paintText(deviceContext, stageText);

		std::string numberText("#: " + std::to_string(currentState.number));
		painter.paintText(deviceContext, numberText);

		std::string realLabelText("real label: " + std::to_string(currentState.realLabel));
		painter.paintText(deviceContext, realLabelText);

		if (currentState.stage == ClassifierStage::test)
		{
			if (!trainCompleted)
			{
				trainCompleted = true;

				setPlotValues(mnistClassifier.getClassPixelDistribution(2, 11, 20));
			}

			std::string predictedLabelText("predicted label: " + std::to_string(currentState.predictedLabel));
			painter.paintText(deviceContext, predictedLabelText);

			std::string successRateText("success rate: " + std::to_string(currentState.successRate));
			painter.paintText(deviceContext, successRateText);

			painter.paintPlot(deviceContext, plotValues, maxPlotValue);
		}
	}
}

void MessageHandler::setPlotValues(std::vector<double> _plotValues)
{
	std::transform(
		_plotValues.begin(),
		_plotValues.end(),
		plotValues.begin(),
		[this](const double plotValue)
		{
			if (maxPlotValue < plotValue)
			{
				maxPlotValue = plotValue;
			}

			return plotValue;
		});
}

void MessageHandler::onClose()
{
	running.store(false);
}