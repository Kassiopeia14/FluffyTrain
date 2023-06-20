#pragma once

#include <windows.h>

class Application
{
private:

	const HINSTANCE instance;

public:

	Application(const HINSTANCE _instance);

	~Application();

	ATOM registerMainWindowClass(WNDPROC mainWindowProcedure);

	WPARAM run();

};