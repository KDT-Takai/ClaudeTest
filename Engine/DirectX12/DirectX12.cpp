#include "DirectX12Window.h"

// コンストラクタ実装
DXWindow::DXWindow(const char* title, int width, int height) 
    : Window(title, width, height), m_nWidth(width), m_nHeight(height)
{
}

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
    m_pSwapChain = CD3DX12CreateSwapChain(
        wcex.hInstance,
        wcex.lpfnWndProc,
        (HWND)GetWindowLongPtr(hwnd),
        &scd);

    if (!m_pSwapChain) {
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
    m_pSwapChain->Initialize(
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
    if (m_pSwapChain) {
        m_pSwapChain->Release();
        m_pSwapChain = nullptr;
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
    m_pSwapChain->ResizeBuffers(
        0,
        newWidth,
        newHeight,
        DXGI_FORMAT_R8G8B8A8_UNORM,
        1);
}