#include "Application.h"

Application::Application(const HINSTANCE _instance):
    instance(_instance)
{
}

Application::~Application()
{
}

LRESULT CALLBACK Application::MainWindowProcedure(
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

ATOM Application::registerMainWindowClass()
{
    WNDCLASS wc;

    wc.style = 0;
    wc.lpfnWndProc = (WNDPROC)MainWindowProcedure;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = instance;
    wc.hIcon = LoadIcon((HINSTANCE)NULL,
        IDI_APPLICATION);
    wc.hCursor = LoadCursor((HINSTANCE)NULL,
        IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = L"MainMenu";
    wc.lpszClassName = L"MainWndClass";

    if (!RegisterClass(&wc))
        return FALSE;
}

WPARAM Application::run()
{
    MSG msg;
    BOOL bRet;

    while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
    {
        if (bRet == -1)
        {
        }
        else
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return msg.wParam;
}