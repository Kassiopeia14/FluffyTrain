#include "Application.h"

Application::Application()
{
}

Application::~Application()
{
}

WPARAM Application::run()
{
    MSG msg;
    BOOL bRet;

    // Start the message loop. 

    while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
    {
        if (bRet == -1)
        {
            // handle the error and possibly exit
        }
        else
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return msg.wParam;
}