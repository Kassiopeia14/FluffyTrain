#include "Brush.h"

Brush::Brush(COLORREF color):
	handle(CreateSolidBrush(color))
{
}

Brush::~Brush()
{
	DeleteObject(handle);
}

HBRUSH Brush::getHandle() const
{
	return handle;
}