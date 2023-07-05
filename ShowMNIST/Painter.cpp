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

Painter::Painter(const size_t _imageSide):
	imageSide(_imageSide),
	grayColors(makeGrayColors()),
	grayBrushes(grayColors.begin(), grayColors.end())
{
}

Painter::~Painter()
{
}

void Painter::paintImage(
	HDC deviceContext,
	std::vector<unsigned char> imageVector)
{
	for (int x = 0; x < imageSide; x++)
	{
		for (int y = 0; y < imageSide; y++)
		{
			const size_t index = y * imageSide + x;

			const unsigned char color = imageVector[index];

			if (color > 0)
			{
				const RECT retangle{
					.left = (LONG)(x * pixelSide),
					.top = (LONG)(y * pixelSide),
					.right = (LONG)(x * pixelSide + pixelSide),
					.bottom = (LONG)(y * pixelSide + pixelSide) };

				FillRect(deviceContext, &retangle, grayBrushes[color].getHandle());
			}
		}
	}
}

void Painter::beginText()
{
	yTextOffset = 8;
}

void Painter::paintText(
	HDC deviceContext,
	const std::string text)
{
	SetBkMode(deviceContext, TRANSPARENT);

	SetTextColor(deviceContext, RGB(255, 0, 0));

	TextOutA(deviceContext, imageSide * pixelSide, yTextOffset, &text[0], text.length());

	yTextOffset += 16;
}

void Painter::paintPlot(HDC deviceContext, std::vector<double> plotValues, double maxPlotValue)
{
	for (int i = 0; i < plotValues.size(); i++)
	{
		double y = plotValues[i] / maxPlotValue * 280;

		SetPixel(deviceContext, 16 + i, imageSide * pixelSide + 280 - y, RGB(255, 255, 255));
	}
}