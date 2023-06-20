#include "Window.h"

Window::Window(HINSTANCE applicationInstance):
    handle(CreateWindow(L"MainWndClass", L"BasicWindow",
        WS_OVERLAPPEDWINDOW, 600, 350,
        800, 500, (HWND)NULL,
        (HMENU)NULL, applicationInstance, (LPVOID)NULL))
{
}

Window::~Window()
{
}

void Window::show(int showCommand)
{
    ShowWindow(handle, showCommand);
}

void Window::update()
{
    UpdateWindow(handle);
}
