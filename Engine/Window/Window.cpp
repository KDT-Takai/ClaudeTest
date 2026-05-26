#include "../Window/Window.h"
#include <windowsx.h>

// グローバル変数 - DirectX12 で共有
WNDCLASSEX g_wcex;

// ウィンドウプロシージャの定義
LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

// DXWindow のウィンドウプロシージャ
LRESULT CALLBACK DXWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

// メンバ関数のインライン実装
inline HWND Window::GetHwnd() const { return hwnd; }
