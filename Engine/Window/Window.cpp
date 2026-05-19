#include "Window.h"

#include "Window.h"

// WNDCLASSEX の初期化 - 正確なフィールド順序と型に注意
WNDCLASSEX wcex = {
    sizeof(WNDCLASSEX),              // cbSize
    CS_HREDRAW | CS_VREDRAW,         // style
    nullptr,                         // lpfnWndProc - 後で代入
    0,                               // cbClsExtra
    0,                               // cbWndExtra
    GetModuleHandle(0),              // hInstance
    LoadIcon(0, IDI_APPLICATION),    // hIcon
    LoadCursor(0, IDC_ARROW),        // hCursor
    (HBRUSH)(COLOR_WINDOW + 1),      // hbrBackground
    0,                               // lpszMenuName
    L"DirectX12GameWindow",          // lpszClassName - LPCWSTR で初期化
    0                                // hIconSm
};

// 静的メンバー宣言と実装 - extern "C" で囲むことで正しく解析される
extern "C" {
    LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        return ::DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

// WNDPROC の代入 - 構造体が完全に初期化された後
wcex.lpfnWndProc = (WNDPROC)Window::WndProc;

Window::~Window() {
    if (hwnd) {
        DestroyWindow(hwnd);
        UnregisterClass(wcex.lpszClassName, wcex.hInstance);
    }
}

bool Window::Create() {
    // クラスを登録する
    if (!RegisterClassEx(&wcex)) {
        OutputDebugStringA("Window::Create failed - RegisterClassEx returned NULL");
        return false;
    }

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