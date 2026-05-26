#pragma once

#include <windows.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include "Window.h"  // 既存のウィンドウクラス

// DirectX12 のコンテキスト設定
enum class D3DContextType : UINT
{
    D3D_CONTEXT_TYPE_DIRECT = 0,
    D3D_CONTEXT_TYPE_SOFTWARE = 1,
};

// DirectX12 ウィンドウクラス
class DXWindow : public Window
{
// Constructor and Destructor
DXWindow(HWND hWnd, int width, int height, const wchar_t* title);
virtual ~DXWindow();

    // コンテキストの作成と初期化
    bool CreateContext(D3DContextType type = D3D_CONTEXT_TYPE_DIRECT);
    
    // リソース管理
    void ResetResources();
    void ResizeBuffers(int newWidth, int newHeight);

    // 描画ループ
    virtual void Render() = 0;

protected:
    ID3D12Device* m_pDevice = nullptr;
    ID3D12CommandQueue* m_pCommandQueue = nullptr;
    ID3D12GraphicsCommandList* m_pCommandList = nullptr;
    IDXGISwapChain* m_pSwapChain = nullptr;

    UINT m_nBackBufferCount = 2;
    int m_nWidth, m_nHeight;
};

// コンストラクタ実装
DXWindow::DXWindow(HWND hWnd, int width, int height) 
    : Window(hWnd, width, height), m_nWidth(width), m_nHeight(height)
{
}

// デストラクタ実装
DXWindow::~DXWindow()
{
    ResetResources();
}

// コンテキストの作成と初期化
bool DXWindow::CreateContext(D3DContextType type)
{
    // 1. DXGI デバイスを作成
    DXGI_SWAP_CHAIN_DESC scd;
    ZeroMemory(&scd, sizeof(scd));
    
    scd.BufferCount = m_nBackBufferCount;
    scd.BufferDesc.Width = m_nWidth;
    scd.BufferDesc.Height = m_nHeight;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.RefreshRate.Numerator = 60;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.SampleDesc.Count = 1;
    scd.Windowed = TRUE;

    // 2. SwapChain を作成
    m_pSwapChainBuffer = CD3DX12CreateSwapChain(
        wcex.hInstance,
        wcex.lpfnWndProc,
        (HWND)GetWindowLongPtr(hwnd),
        &scd);

    if (!m_pSwapChainBuffer) {
        OutputDebugStringA("DXWindow::CreateContext failed - SwapChain creation");
        return false;
    }

    // 3. D3D12 デバイスを作成
    m_pDevice = CD3DX12CreateDevice(
        wcex.hInstance,
        &scd);

    if (!m_pDevice) {
        OutputDebugStringA("DXWindow::CreateContext failed - Device creation");
        return false;
    }

    // 4. CommandQueue を作成
    m_pCommandQueue = CD3DX12CreateCommandQueue(
        wcex.hInstance,
        &scd);

    if (!m_pCommandQueue) {
        OutputDebugStringA("DXWindow::CreateContext failed - CommandQueue creation");
        return false;
    }

    // 5. CommandList を作成
    m_pCommandList = CD3DX12CreateCommandList(
        wcex.hInstance,
        &scd);

    if (!m_pCommandList) {
        OutputDebugStringA("DXWindow::CreateContext failed - CommandList creation");
        return false;
    }

    // 6. SwapChain を初期化
    m_pSwapChainBuffer->Initialize(
        wcex.hInstance,
        wcex.lpfnWndProc,
        (HWND)GetWindowLongPtr(hwnd),
        &scd);

    OutputDebugStringA("DXWindow::CreateContext succeeded");
    return true;
}

// リソース管理
void DXWindow::ResetResources()
{
    if (m_pSwapChainBuffer) {
        m_pSwapChainBuffer->Release();
        m_pSwapChainBuffer = nullptr;
    }
    
    if (m_pDevice) {
        m_pDevice->Release();
        m_pDevice = nullptr;
    }

    if (m_pCommandQueue) {
        m_pCommandQueue->Release();
        m_pCommandQueue = nullptr;
    }

    if (m_pCommandList) {
        m_pCommandList->Release();
        m_pCommandList = nullptr;
    }
}

// リサイズ処理
void DXWindow::ResizeBuffers(int newWidth, int newHeight)
{
    m_nWidth = newWidth;
    m_nHeight = newHeight;

    // SwapChain のバッファをリサイズ
    if (m_pSwapChainBuffer) {
        m_pSwapChainBuffer->ResizeBuffers(
            0,
            newWidth,
            newHeight,
            DXGI_FORMAT_R8G8B8A8_UNORM,
            1);
    }
}