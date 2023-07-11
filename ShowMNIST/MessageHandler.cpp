#include "MessageHandler.h"

MessageHandler::MessageHandler(
	MNISTClassifier<SoftmaxEngine>& _mnistClassifier,
	std::atomic<bool>& _running):
	painter(MNISTLoader::imageSide),
	mnistClassifier(_mnistClassifier),
	trainCompleted(false),
	plotValues(256, 0.),
	maxPlotValue(1.),
	running(_running),
	yellow(RGB(255, 255, 0)),
	red(RGB(255, 0, 0))
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

		std::string engineName("Engine: " + mnistClassifier.getEngineName());
		painter.paintText(deviceContext, engineName, red);

		std::string stageText("stage: " + (currentState.stage == ClassifierStage::train ? std::string("TRAIN") : std::string("TEST")));
		painter.paintText(deviceContext, stageText, red);

		if (currentState.tested)
		{
			std::string successRateText("success rate: " + std::to_string(currentState.successRate));
			painter.paintText(deviceContext, successRateText, yellow);
		}

		std::string numberText("#: " + std::to_string(currentState.number));
		painter.paintText(deviceContext, numberText, red);

		std::string realLabelText("real label: " + std::to_string(currentState.realLabel));
		painter.paintText(deviceContext, realLabelText, red);

		if (currentState.stage == ClassifierStage::test)
		{
			//if (!trainCompleted)
			//{
			//	trainCompleted = true;

			//	setPlotValues(mnistClassifier.getClassPixelDistribution(2, 11, 20));
			//}

			std::string predictedLabelText("predicted label: " + std::to_string(currentState.predictedLabel));
			painter.paintText(deviceContext, predictedLabelText, red);

			//painter.paintPlot(deviceContext, plotValues, maxPlotValue);
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