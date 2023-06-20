#pragma once

#include <windows.h>

class Window
{
private:

	HWND handle;

	static LRESULT CALLBACK MainWindowProcedure(
		HWND hwnd,
		UINT uMsg,
		WPARAM wParam,
		LPARAM lParam);

public:

	Window(HINSTANCE applicationInstance);

	~Window();

	static WNDPROC getWindowProcedure();

	void show(int showCommand);

	void update();


};