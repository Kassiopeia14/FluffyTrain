#pragma once

#include <windows.h>

template<class MessageHandler> 
class Window
{
private:

    MessageHandler* messageHandler;

    HWND handle;

	static LRESULT CALLBACK MainWindowProcedure(
		HWND hwnd,
		UINT uMsg,
		WPARAM wParam,
		LPARAM lParam);

	static void create(HWND hwnd, LPARAM lParam);

	static bool paint(HWND hwnd);

    static void close(HWND hwnd);

    HWND makeWindow(
        HINSTANCE applicationInstance,
        const LONG clientWidth,
        const LONG clientHeight);

	bool onPaint();

    void onClose();

public:

	Window(
        HINSTANCE applicationInstance,
        const LONG clientWidth,
        const LONG clientHeight,
        MessageHandler* _messageHandler);

	~Window();

	static WNDPROC getWindowProcedure();

	void show(int showCommand);

	void update();

    void refresh();
};

template<class MessageHandler>
Window<MessageHandler>::Window(
    HINSTANCE applicationInstance,
    const LONG clientWidth,
    const LONG clientHeight,
    MessageHandler* _messageHandler) :
    messageHandler(_messageHandler),
    handle(makeWindow(
        applicationInstance,
        clientWidth,
        clientHeight))
{
}

template<class MessageHandler>
Window<MessageHandler>::~Window()
{
}

template<class MessageHandler>
HWND Window<MessageHandler>::makeWindow(
    HINSTANCE applicationInstance,
    const LONG clientWidth,
    const LONG clientHeight)
{
    RECT rect{
        .left = 0,
        .top = 0,
        .right = clientWidth,
        .bottom = clientHeight
    };

    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

    HWND windowHandle = CreateWindow(L"MainWndClass", L"BasicWindow",
        WS_OVERLAPPEDWINDOW, 600, 350,
        rect.right - rect.left, rect.bottom - rect.top, (HWND)NULL,
        (HMENU)NULL, applicationInstance, (LPVOID)this);

    return windowHandle;
}

template<class MessageHandler>
WNDPROC Window<MessageHandler>::getWindowProcedure()
{
    return (WNDPROC)MainWindowProcedure;
}

template<class MessageHandler>
LRESULT CALLBACK Window<MessageHandler>::MainWindowProcedure(
    HWND hwnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        create(hwnd, lParam);
        return 0;

    case WM_PAINT:
        paint(hwnd);
        return 0;

    case WM_SIZE:
        return 0;

    case WM_CLOSE:
        close(hwnd);
        return 0;

    case WM_DESTROY:
        return 0;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

template<class MessageHandler>
void Window<MessageHandler>::create(HWND hwnd, LPARAM lParam)
{
    CREATESTRUCT* createData = (CREATESTRUCT*)lParam;

    Window* window = (Window*)createData->lpCreateParams;

    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);
}

template<class MessageHandler>
bool Window<MessageHandler>::paint(HWND hwnd)
{
    Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    return window->onPaint();
}

template<class MessageHandler>
void Window<MessageHandler>::close(HWND hwnd)
{
    Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    window->onClose();

    PostQuitMessage(0);
}

template<class MessageHandler>
bool Window<MessageHandler>::onPaint()
{
    PAINTSTRUCT paintData;

    HDC deviceContext = BeginPaint(handle, &paintData);

    messageHandler->onPaint(deviceContext);

    return EndPaint(handle, &paintData);
}

template<class MessageHandler>
void Window<MessageHandler>::onClose()
{
    messageHandler->onClose();
}

template<class MessageHandler>
void Window<MessageHandler>::show(int showCommand)
{
    ShowWindow(handle, showCommand);
}

template<class MessageHandler>
void Window<MessageHandler>::update()
{
    UpdateWindow(handle);
}

template<class MessageHandler>
void Window<MessageHandler>::refresh()
{
    InvalidateRect(handle, 0, true);
}
