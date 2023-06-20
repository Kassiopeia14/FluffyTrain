#include "Application.h"

HINSTANCE hinst;
HWND hwndMain;

LRESULT CALLBACK WndProc(
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

int APIENTRY WinMain(
    HINSTANCE hInstance, 
    HINSTANCE hPrevInstance,
    LPSTR lpszCmdLine, 
    int nCmdShow)
{
    Application application;

    WNDCLASS wc;
    UNREFERENCED_PARAMETER(lpszCmdLine);

    if (!hPrevInstance)
    {
        wc.style = 0;
        wc.lpfnWndProc = (WNDPROC)WndProc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = hInstance;
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

    hinst = hInstance;  

    hwndMain = CreateWindow(L"MainWndClass", L"BasicWindow",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT, (HWND)NULL,
        (HMENU)NULL, hinst, (LPVOID)NULL);

    if (!hwndMain)
        return FALSE;

    ShowWindow(hwndMain, nCmdShow);
    UpdateWindow(hwndMain);

    return application.run();
}