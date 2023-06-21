#include "MNISTMessageHandler.h"

MNISTMessageHandler::MNISTMessageHandler():
	//fileName("C:\\Github\\FluffyTrain\\mnist\\t10k-images.idx3-ubyte"),
	fileName("C:\\Github\\FluffyTrain\\mnist\\train-images.idx3-ubyte"),
	mnistLoader(fileName)
{
}

MNISTMessageHandler::~MNISTMessageHandler()
{
}

void MNISTMessageHandler::onPaint(HDC deviceContext)
{
	painter.paintImage(deviceContext, MNISTLoader::imageSide, mnistLoader.getImage(0));
}
