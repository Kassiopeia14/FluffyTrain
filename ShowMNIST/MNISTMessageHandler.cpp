#include "MNISTMessageHandler.h"

MNISTMessageHandler::MNISTMessageHandler()
{
}

MNISTMessageHandler::~MNISTMessageHandler()
{
}

void MNISTMessageHandler::onPaint(HDC deviceContext)
{
    const RECT retangle{ .left = 100, .top = 100, .right = 200, .bottom = 300 };

    HBRUSH brush = CreateSolidBrush(RGB(172, 35, 230));

    FillRect(deviceContext, &retangle, brush);
}
