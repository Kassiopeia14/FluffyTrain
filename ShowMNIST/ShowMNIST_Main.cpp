#include "../modWindows/Application.h"
#include "../modWindows/Window.h"
#include "MessageHandler.h"
#include "../modMNIST/MNISTLoader.h"
#include "../modMNISTClassifier/MNISTClassifier.h"

int APIENTRY WinMain(
    HINSTANCE hInstance, 
    HINSTANCE hPrevInstance,
    LPSTR lpszCmdLine, 
    int nCmdShow)
{
    Application application(hInstance);

    application.registerMainWindowClass(Window<MessageHandler>::getWindowProcedure());

    std::string path("C:\\Github\\FluffyTrain\\mnist\\");
    MNISTLoader mnistLoader(path);

    RandomEngine randomEngine;

    std::atomic<bool> running(true);

    MNISTClassifier<RandomEngine> mnistClassifier(randomEngine, mnistLoader, running);

    MessageHandler messageHandler(mnistClassifier, running);

    Window<MessageHandler> mainWindow(
        hInstance, 
        (LONG)(MNISTLoader::imageSide * Painter::pixelSide) + 300,
        (LONG)(MNISTLoader::imageSide * Painter::pixelSide),
        &messageHandler);

    mainWindow.show(nCmdShow);

    mainWindow.update();

    return application.run([&]() -> void 
    {
        mainWindow.refresh();
    });
}