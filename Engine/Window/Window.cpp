#include "Window.h"

Window::~Window() {
    if (hwnd) {
        DestroyWindow(hwnd);
        UnregisterClass(wcex.lpszClassName, wcex.hInstance);
    }
}

bool Window::Create() {
    if (!RegisterClassEx(&wcex)) {
        return false;
    }

    wchar_t wtitle[256];
    MultiByteToWideChar(CP_ACP, 0, title, -1, wtitle, sizeof(wtitle));

    hwnd = CreateWindowEx(
        0,
        wcex.lpszClassName,
        wtitle,
        WS_OVERLAPPEDWINDOW,
        100, 100,
        width,
        height,
        nullptr,
        nullptr,
        wcex.hInstance,
        nullptr
    );

    if (!hwnd) {
        return false;
    }

    // WM_CREATE で this をウィンドウに保持
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);

    return true;
}

bool Window::Run() {
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
}

bool Window::ProcessMessage(UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_QUIT:
        return false;
    default:
        return true;
    }
}