#include "MessageHandler.h"

MessageHandler::MessageHandler(
	MNISTClassifier<RandomEngine>& _mnistClassifier,
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

		std::string predictedLabelText("predicted label: " + std::to_string(currentState.predictedLabel));
		painter.paintText(deviceContext, predictedLabelText);

		std::string realLabelText("real label: " + std::to_string(currentState.realLabel));
		painter.paintText(deviceContext, realLabelText);

		std::string numberText("#: " + std::to_string(currentState.number));
		painter.paintText(deviceContext, numberText);
	}
}

void MessageHandler::onClose()
{
	running.store(false);
}