#include "../modWindows/Application.h"
#include "../modWindows/Window.h"
#include "MNISTMessageHandler.h"

int APIENTRY WinMain(
    HINSTANCE hInstance, 
    HINSTANCE hPrevInstance,
    LPSTR lpszCmdLine, 
    int nCmdShow)
{
    Application application(hInstance);

    application.registerMainWindowClass(Window<MNISTMessageHandler>::getWindowProcedure());

    MNISTMessageHandler mnistMessageHandler;

    Window<MNISTMessageHandler> mainWindow(hInstance, &mnistMessageHandler);
    Window<MNISTMessageHandler> mainWindow1(hInstance, &mnistMessageHandler);

    mainWindow.show(nCmdShow);

    mainWindow.update();

    mainWindow1.show(nCmdShow);

    mainWindow1.update();


    return application.run();
}