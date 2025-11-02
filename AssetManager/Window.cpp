#include "Window.h"


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    
    default:
        return DefWindowProc(hwnd, message, wparam, lparam);
    }
    return 0;
}

Window::Window() {
}
void Window::initializeClass() {
    HINSTANCE hInstance = GetModuleHandle(NULL);

	wc = { 0 };
    wc.hbrBackground = CreateSolidBrush(RGB(255, 0, 0));
	wc.hInstance = hInstance;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = L"My Window Abstraction";
    if (!RegisterClass(&wc))
        MessageBox(NULL, L"Could not register class", L"Error", MB_OK);
}
void Window::initializeWindow() {
    windowHandle = CreateWindowEx(
        0,
        wc.lpszClassName,
        L"AssetManager",
        WS_OVERLAPPEDWINDOW,
        0, 0, 1280, 720,
        NULL,
        NULL,
        wc.hInstance,
        NULL
    );

    ShowWindow(windowHandle, SW_SHOW);
}

void Window::terminateWindow() {
    DeleteObject(windowHandle);
}
void Window::terminateClass() {

}

bool Window::shouldWindowClose() {
    return !(GetMessage(&messages, windowHandle, 0, 0) > 0);
}
void Window::pollEvents() {
    TranslateMessage(&messages);
    DispatchMessage(&messages);
}

HWND& Window::getHWND() {
    return windowHandle;
}
WNDCLASS& Window::getWindowClass() {
    return wc;
}