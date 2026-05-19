#pragma once

#include <Windows.h>

class Window {
private:
    HWND hwnd;
    WNDCLASSEX wcex;
    const char* title;
    int width;
    int height;

    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
    Window(const char* title, int width, int height) : title(title), width(width), height(height) {}
    ~Window();

    bool Create();
    bool Run();
    HWND GetHwnd() const;
    bool ProcessMessage(UINT msg, WPARAM wParam, LPARAM lParam);
};