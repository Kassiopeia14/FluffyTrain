#pragma once

#include <vector>
#include <windows.h>

class Painter
{
private:

	static const size_t pixelSide = 8;

public:
	
	Painter();
	~Painter();

	void paintImage(
		HDC deviceContext,
		const size_t imageSide,
		std::vector<char> imageVector);

};