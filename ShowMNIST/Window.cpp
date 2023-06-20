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

WNDPROC Window::getWindowProcedure()
{
    return (WNDPROC)MainWindowProcedure;
}

LRESULT CALLBACK Window::MainWindowProcedure(
    HWND hwnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        return 0;

    case WM_PAINT:
        return 0;

    case WM_SIZE:
        return 0;

    case WM_DESTROY:
        return 0;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}


void Window::show(int showCommand)
{
    ShowWindow(handle, showCommand);
}

void Window::update()
{
    UpdateWindow(handle);
}
