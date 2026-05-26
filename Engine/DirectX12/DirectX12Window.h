#pragma once

#include <windows.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl/client.h>
#include "../Renderer/ID3D12GraphicsCommandList.h"

using namespace Microsoft::WRL;

// Forward declarations
struct CD3DX12_CLEAR_VALUE
{
    DXGI_FORMAT Format;
    FLOAT ClearValue[4];
};

class ID3D12GraphicsCommandList;

// DirectX 12 Window Class
class DXWindow : public IUnknown
{
public:
    // Constructor and Destructor
    DXWindow(HWND hWnd, int width, int height, const wchar_t* title);
    virtual ~DXWindow();

    // Initialize the window
    HRESULT Initialize(HWND hWnd, int width, int height, const wchar_t* title);

    // Main render loop function
    void RenderLoop();

    // Getters for convenience
    inline HWND GetHWND() const { return m_hWnd; }
    inline UINT32 GetWidth() const { return m_nWidth; }
    inline UINT32 GetHeight() const { return m_nHeight; }

    // Check if window is active
    bool IsActive() const { return m_active; }

protected:
    // Window handle
    HWND m_hWnd;
    
    // Window dimensions
    UINT32 m_nWidth;
    UINT32 m_nHeight;
    
    // Active flag
    bool m_active;

    // DirectX 12 objects (protected for derived classes)
    ComPtr<ID3D12Device> m_device;
    ComPtr<ID3D12CommandQueue> m_commandQueue;
    ComPtr<ID3D12GraphicsCommandList4> m_commandList;
    ComPtr<IDXGISwapChain1> m_swapChain;

    // Clear value for rendering (member variable)
    CD3DX12_CLEAR_VALUE clearValue;

    // Swap chain back buffer handle
    HANDLE m_pSwapChainBuffer;  // Renamed from m_pSwapChain to avoid confusion

    // Window procedure callback
    WNDPROC lpfnWndProc;

private:
    // Helper functions
    HRESULT CreateDevice();
    HRESULT CreateCommandQueueAndList();
    HRESULT CreateSwapChain(HWND hWnd, UINT32 width, UINT32 height);
    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
