#pragma once

#include <windows.h>

class DXWindow;  // 前方宣言 - DirectX12Window で使用

// グローバル変数 - DirectX12 で共有
extern WNDCLASSEX g_wcex;

// ウィンドウクラス
class Window {
private:
    HWND hwnd;
    WNDCLASSEX wcex;
    const char* title;
    int width;
    int height;

    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
    // コンストラクタ - wcex を初期化
    Window(const char* title, int width, int height) 
        : title(title), width(width), height(height) 
    {
        // ウィンドウクラスを初期化
        ZeroMemory(&wcex, sizeof(WNDCLASSEX));
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = Window::WndProc;
        wcex.hInstance = GetModuleHandle(NULL);
        wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszClassName = "DirectX12WindowClass";
        wcex.lpfnWndProc = (WNDPROC)DXWindow::WndProc;  // DXWindow のプロシージャを使用
    }
    
    ~Window() {
        if (hwnd) {
            UnregisterClass(wcex.lpszClassName, wcex.hInstance);
        }
    }

    bool Create();
    bool Run();
    HWND GetHwnd() const;
    bool ProcessMessage(UINT msg, WPARAM wParam, LPARAM lParam);
};
