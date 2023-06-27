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
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
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
            
            std::chrono::duration<double> duration = checkPoint - begin;
            
            if (duration.count() > 0.2)
            {
                begin = checkPoint;

                task();
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::microseconds(10));
            }
        }
    }

    return msg.wParam;
}