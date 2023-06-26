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

	std::array<COLORREF, colorCount> grayColors;

	std::vector<Brush> grayBrushes;

	static std::array<COLORREF, colorCount> makeGrayColors();

public:
	
	Painter();
	~Painter();

	void paintImage(
		HDC deviceContext,
		const size_t imageSide,
		std::vector<unsigned char> imageVector);
	
	void paintText(
		HDC deviceContext,
		const std::string text);

};