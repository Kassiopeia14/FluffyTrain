#include "Application.h"

HWND hwndMain;

int APIENTRY WinMain(
    HINSTANCE hInstance, 
    HINSTANCE hPrevInstance,
    LPSTR lpszCmdLine, 
    int nCmdShow)
{
    Application application(hInstance);

    application.registerMainWindowClass();

    hwndMain = CreateWindow(L"MainWndClass", L"BasicWindow",
        WS_OVERLAPPEDWINDOW, 600, 350,
        800, 500, (HWND)NULL,
        (HMENU)NULL, hInstance, (LPVOID)NULL);

    if (!hwndMain)
        return FALSE;

    ShowWindow(hwndMain, nCmdShow);
    UpdateWindow(hwndMain);

    return application.run();
}