#include "Painter.h"

std::array<COLORREF, Painter::colorCount> Painter::makeGrayColors()
{
	std::array<COLORREF, colorCount> result;

	for (int i = 0; i < colorCount; i++)
	{
		result[i] = RGB(i, i, i);
	}

	return result;
}

Painter::Painter():
	grayColors(makeGrayColors()),
	grayBrushes(grayColors.begin(), grayColors.end())
{
}

Painter::~Painter()
{
}

void Painter::paintImage(
	HDC deviceContext,
	const size_t imageSide,
	std::vector<unsigned char> imageVector)
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

			const unsigned char color = imageVector[index];

			FillRect(deviceContext, &retangle, grayBrushes[color].getHandle());
		}
	}
}