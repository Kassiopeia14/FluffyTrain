#pragma once

#include <windows.h>

class Application
{
private:

	const HINSTANCE instance;

	static LRESULT CALLBACK MainWindowProcedure(
		HWND hwnd,
		UINT uMsg,
		WPARAM wParam,
		LPARAM lParam);

public:

	Application(const HINSTANCE _instance);

	~Application();

	ATOM registerMainWindowClass();

	WPARAM run();

};