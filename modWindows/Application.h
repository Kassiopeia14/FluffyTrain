#pragma once

#include <windows.h>
#include <functional>
#include <chrono>
#include <thread>

class Application
{
private:

	const HINSTANCE instance;

public:

	Application(const HINSTANCE _instance);

	~Application();

	ATOM registerMainWindowClass(WNDPROC mainWindowProcedure);

	WPARAM run(std::function<void()> task);

};