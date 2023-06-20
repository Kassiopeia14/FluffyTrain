#include "Application.h"

Application::Application(const HINSTANCE _instance):
    instance(_instance)
{
}

Application::~Application()
{
}


ATOM Application::registerMainWindowClass(WNDPROC mainWindowProcedure)
{
    WNDCLASS wc;

    wc.style = 0;
    wc.lpfnWndProc = mainWindowProcedure;
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