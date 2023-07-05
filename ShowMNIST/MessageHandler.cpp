#include "MessageHandler.h"

MessageHandler::MessageHandler(
	MNISTClassifier<NaiveBayesGaussianDistributionEngine>& _mnistClassifier,
	std::atomic<bool>& _running):
	painter(MNISTLoader::imageSide),
	mnistClassifier(_mnistClassifier),
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
			std::string predictedLabelText("predicted label: " + std::to_string(currentState.predictedLabel));
			painter.paintText(deviceContext, predictedLabelText);

			std::string successRateText("success rate: " + std::to_string(currentState.successRate));
			painter.paintText(deviceContext, successRateText);
		}
	}
}

void MessageHandler::onClose()
{
	running.store(false);
}