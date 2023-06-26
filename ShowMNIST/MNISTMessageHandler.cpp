#include "MNISTMessageHandler.h"

MNISTMessageHandler::MNISTMessageHandler():
	fileName("C:\\Github\\FluffyTrain\\mnist\\t10k-images.idx3-ubyte"),
	//fileName("C:\\Github\\FluffyTrain\\mnist\\train-images.idx3-ubyte"),
	imageCounter(100),
	mnistLoader(fileName)
{
}

MNISTMessageHandler::~MNISTMessageHandler()
{
}

void MNISTMessageHandler::onPaint(HDC deviceContext)
{
	imageCounter = (imageCounter + 1) % 10000;

	painter.paintImage(deviceContext, MNISTLoader::imageSide, mnistLoader.getImage(imageCounter));

	painter.paintText(deviceContext, "abv");
}
