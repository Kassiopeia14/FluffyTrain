#pragma once

#include <windows.h>

class Brush
{
private:

	HBRUSH handle;

public:

	Brush(COLORREF color);
	~Brush();

	HBRUSH getHandle() const;
};