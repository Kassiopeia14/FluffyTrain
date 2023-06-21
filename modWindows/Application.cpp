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

WPARAM Application::run(std::function<void()> task)
{
    MSG msg = {};
    BOOL bRet;

    std::chrono::time_point<std::chrono::system_clock> begin = std::chrono::system_clock::now();

    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            std::chrono::time_point<std::chrono::system_clock> checkPoint = std::chrono::system_clock::now();

            if ((checkPoint - begin).count() > 10000000.0)
            {
                begin = checkPoint;

                task();
            }
        }
    }

    return msg.wParam;
}