#pragma once

#include <windows.h>
#include "Painter.h"
#include "../modMNIST/MNISTLoader.h"

class MNISTMessageHandler
{
private:

	std::string fileName;
	MNISTLoader mnistLoader;

	Painter painter;

public:
	MNISTMessageHandler();
	~MNISTMessageHandler();

	void onPaint(HDC deviceContext);

};