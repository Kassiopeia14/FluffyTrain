#include "Application.h"
#include "Window.h"

HWND hwndMain;

int APIENTRY WinMain(
    HINSTANCE hInstance, 
    HINSTANCE hPrevInstance,
    LPSTR lpszCmdLine, 
    int nCmdShow)
{
    Application application(hInstance);

    application.registerMainWindowClass();

    Window mainWindow(hInstance);

    mainWindow.show(nCmdShow);

    mainWindow.update();

    return application.run();
}