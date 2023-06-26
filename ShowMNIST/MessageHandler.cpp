#include "MessageHandler.h"

MessageHandler::MessageHandler(
	MNISTClassifier<RandomEngine>& _mnistClassifier,
	std::atomic<bool>& _running):
	mnistClassifier(_mnistClassifier),
	running(_running)
{
}

MessageHandler::~MessageHandler()
{
}

void MessageHandler::onPaint(HDC deviceContext)
{
	ClassifiedImage currentImage(mnistClassifier.getCurrentImage());

	painter.paintImage(deviceContext, MNISTLoader::imageSide, currentImage.image);

	painter.paintText(deviceContext, std::to_string(currentImage.label));
}

void MessageHandler::onClose()
{
	running.store(false);
}