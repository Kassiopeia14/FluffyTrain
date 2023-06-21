#include "Painter.h"

Painter::Painter()
{
}

Painter::~Painter()
{
}


void Painter::paintImage(
	HDC deviceContext,
	const size_t imageSide,
	std::vector<char> imageVector)
{
	for (int x = 0; x < imageSide; x++)
	{
		for (int y = 0; y < imageSide; y++)
		{
			const RECT retangle{ 
				.left = (LONG)(x * pixelSide), 
				.top = (LONG)(y * pixelSide), 
				.right = (LONG)(x * pixelSide + pixelSide), 
				.bottom = (LONG)(y * pixelSide + pixelSide) };

			const size_t index = y * imageSide + x;

			const char color = imageVector[index];

			HBRUSH brush = CreateSolidBrush(RGB(color, color, color));

			FillRect(deviceContext, &retangle, brush);

			DeleteObject(brush);
		}
	}
}
