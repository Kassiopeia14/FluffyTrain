#include "Application.h"

HINSTANCE hinst;
HWND hwndMain;

LRESULT CALLBACK WndProc(
    HWND hwnd,        // handle to window
    UINT uMsg,        // message identifier
    WPARAM wParam,    // first message parameter
    LPARAM lParam)    // second message parameter
{

    switch (uMsg)
    {
    case WM_CREATE:
        // Initialize the window. 
        return 0;

    case WM_PAINT:
        // Paint the window's client area. 
        return 0;

    case WM_SIZE:
        // Set the size and position of the window. 
        return 0;

    case WM_DESTROY:
        // Clean up window-specific data objects. 
        return 0;

        // 
        // Process other messages. 
        // 

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

    // Register the window class for the main window. 

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

    hinst = hInstance;  // save instance handle 

    // Create the main window. 

    hwndMain = CreateWindow(L"MainWndClass", L"BasicWindow",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT, (HWND)NULL,
        (HMENU)NULL, hinst, (LPVOID)NULL);

    // If the main window cannot be created, terminate 
    // the application. 

    if (!hwndMain)
        return FALSE;

    // Show the window and paint its contents. 

    ShowWindow(hwndMain, nCmdShow);
    UpdateWindow(hwndMain);


    return application.run();

    // Return the exit code to the system. 
}