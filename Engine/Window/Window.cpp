#include "Window.h"

// WNDCLASSEX の初期化 - 正確なフィールド順序に注意
static const WNDCLASSEX wcex = {
    sizeof(WNDCLASSEX),              // cbSize
    CS_HREDRAW | CS_VREDRAW,         // style
    (WNDPROC)Window::WndProc,        // lpfnWndProc
    0,                               // cbClsExtra
    0,                               // cbWndExtra
    GetModuleHandle(0),              // hInstance
    LoadIcon(0, IDI_APPLICATION),    // hIcon
    LoadCursor(0, IDC_ARROW),        // hCursor
    (HBRUSH)(COLOR_WINDOW + 1),      // hbrBackground
    0,                               // lpszMenuName
    "DirectX12GameWindow",           // lpszClassName
    0                                // hIconSm
};

// 静的メンバー宣言
LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

Window::~Window() {
    if (hwnd) {
        DestroyWindow(hwnd);
        UnregisterClass(wcex.lpszClassName, wcex.hInstance);
    }
}

bool Window::Create() {
    wchar_t wtitle[256];
    MultiByteToWideChar(CP_ACP, 0, title, -1, wtitle, sizeof(wtitle));

    hwnd = CreateWindowEx(
        0,
        wcex.lpszClassName,
        wtitle,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,  // 初期表示
        100, 100,
        width,
        height,
        nullptr,
        nullptr,
        wcex.hInstance,
        nullptr
    );

    if (!hwnd) {
        OutputDebugStringA("Window::Create failed - CreateWindowEx returned NULL");
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