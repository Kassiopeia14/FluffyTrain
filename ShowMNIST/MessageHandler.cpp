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

	painter.paintImage(deviceContext, currentState.image);

	painter.beginText();
	
	std::string labelText("label: " + std::to_string(currentState.label));
	painter.paintText(deviceContext, labelText);

	std::string numberText("#: " + std::to_string(currentState.number));
	painter.paintText(deviceContext, numberText);


}

void MessageHandler::onClose()
{
	running.store(false);
}