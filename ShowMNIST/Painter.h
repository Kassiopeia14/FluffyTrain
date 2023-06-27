#pragma once

#include <array>
#include <vector>
#include <string>
#include <windows.h>

#include "../modWindows/Brush.h"

class Painter
{
public:

	static const size_t
		colorCount = 256,
		pixelSide = 8;

private:

	const size_t imageSide;

	std::array<COLORREF, colorCount> grayColors;

	std::vector<Brush> grayBrushes;

	size_t yTextOffset;

	static std::array<COLORREF, colorCount> makeGrayColors();

public:
	
	Painter(const size_t _imageSide);

	~Painter();

	void paintImage(
		HDC deviceContext,
		std::vector<unsigned char> imageVector);
	
	void beginText();

	void paintText(
		HDC deviceContext,
		const std::string text);

};