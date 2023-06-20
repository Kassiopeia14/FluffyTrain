#pragma once

#include <windows.h>

class Window
{
private:

	HWND handle;

public:

	Window(HINSTANCE applicationInstance);

	~Window();

	void show(int showCommand);

	void update();
};