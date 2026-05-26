#include "../DirectX12/DirectX12.h"
#include <windowsx.h>

// コンストラクタ実装
DXWindow::DXWindow(HWND hWnd, int width, int height, const wchar_t* title) 
    : m_hWnd(hWnd), m_nWidth(width), m_nHeight(height), m_active(true)
{
    // ウィンドウプロシージャの保存
    lpfnWndProc = (WNDPROC)SetWindowLongPtr(hWnd, GWL_WNDPROC, 
        reinterpret_cast<LONG_PTR>(&DXWindow::WndProc));

    // 初期ウィンドウサイズを設定
    SetWindowPos(hWnd, HWND_TOP, 0, 0, width, height, SWP_NOZORDER);
}

// デストラクタ実装
DXWindow::~DXWindow()
{
    ResetResources();
}

// 作成処理
bool DXWindow::Create()
{
    // ウィンドウスタイルの設定
    DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    
    // タイトルバーの文字列をセット
    wchar_t title[256];
    swprintf(title, 256, L"%s", m_title ? m_title : L"DirectX12 Window");

    // ウィンドウハンドルを取得または作成
    if (!m_hWnd) {
        // 新しいウィンドウを作成
        m_hWnd = CreateWindowExW(
            0,
            L"DirectX12WindowClass",
            title,
            style,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            m_nWidth,
            m_nHeight,
            nullptr,
            nullptr,
            g_wcex.hInstance,
            nullptr);

        if (!m_hWnd) {
            OutputDebugStringA("DXWindow::Create - Window creation failed");
            return false;
        }

        // ウィンドウプロシージャを設定
        lpfnWndProc = (WNDPROC)SetWindowLongPtr(m_hWnd, GWL_WNDPROC, 
            reinterpret_cast<LONG_PTR>(&DXWindow::WndProc));
    }

    OutputDebugStringA("DXWindow::Create succeeded");
    return true;
}

// 初期化処理
HRESULT DXWindow::Initialize(HWND hWnd, int width, int height, const wchar_t* title)
{
    m_hWnd = hWnd;
    m_nWidth = static_cast<UINT32>(width);
    m_nHeight = static_cast<UINT32>(height);
    m_active = true;

    // ウィンドウプロシージャを設定
    lpfnWndProc = (WNDPROC)SetWindowLongPtr(hWnd, GWL_WNDPROC, 
        reinterpret_cast<LONG_PTR>(&DXWindow::WndProc));

    return S_OK;
}

// メインレンダーループ
void DXWindow::RenderLoop()
{
    if (!m_active || !m_swapChain) {
        return;
    }

    // バックバッファのクリア
    ID3D12GraphicsCommandList4* cmdList = m_commandList.Get();
    
    // ClearRenderTargetView の設定
    CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM) clearValue;
    clearValue.Color[0].r = 0.5f;  // 半透明の青
    clearValue.Color[0].g = 0.7f;
    clearValue.Color[0].b = 1.0f;
    clearValue.Color[0].a = 1.0f;

    cmdList->ClearRenderTargetView(
        m_swapChain->GetBackBuffer(0, 0),
        &clearValue);

    // 描画コマンドの実行
    cmdList->Close();

    // SwapChain を更新して画面に反映
    m_swapChain->Present(1, 0);
}

// ヘルパー関数：デバイス作成
HRESULT DXWindow::CreateDevice()
{
    HRESULT hr = S_OK;

    // D3D_FEATURE_LEVEL の配列
    const D3D_FEATURE_LEVEL featureLevels[] = 
    {
        D3D_FEATURE_LEVEL_12_0,
        D3D_FEATURE_LEVEL_12_1,
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };

    // デバイス作成
    hr = CD3DX12CreateDevice(
        GetHWND(),
        featureLevels,
        &m_device);

    if (FAILED(hr)) {
        OutputDebugStringA("DXWindow::CreateDevice failed");
        return hr;
    }

    // コマンドキュー作成
    ComPtr<ID3D12CommandQueue> commandQueue;
    hr = CD3DX12CreateCommandQueue(
        GetHWND(),
        &m_commandQueue);

    if (FAILED(hr)) {
        OutputDebugStringA("DXWindow::CreateDevice - CommandQueue creation failed");
        return hr;
    }

    // コマンドリスト作成
    ComPtr<ID3D12GraphicsCommandList4> commandList;
    hr = CD3DX12CreateCommandList(
        GetHWND(),
        &m_commandList);

    if (FAILED(hr)) {
        OutputDebugStringA("DXWindow::CreateDevice - CommandList creation failed");
        return hr;
    }

    // SwapChain 作成
    ComPtr<IDXGISwapChain1> swapChain;
    DXGI_SWAP_CHAIN_DESC scd = {};
    ZeroMemory(&scd, sizeof(scd));

    scd.BufferCount = 2;
    scd.BufferDesc.Width = m_nWidth;
    scd.BufferDesc.Height = m_nHeight;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.RefreshRate.Numerator = 60;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.SampleDesc.Count = 1;
    scd.Windowed = TRUE;

    hr = CD3DX12CreateSwapChain(
        GetHWND(),
        lpfnWndProc,
        (HWND)GetWindowLongPtr(GetHWND()),
        &scd,
        &swapChain);

    if (FAILED(hr)) {
        OutputDebugStringA("DXWindow::CreateDevice - SwapChain creation failed");
        return hr;
    }

    // SwapChain を初期化
    swapChain->Initialize(
        GetHWND(),
        lpfnWndProc,
        (HWND)GetWindowLongPtr(GetHWND()),
        &scd);

    OutputDebugStringA("DXWindow::CreateDevice succeeded");
    return S_OK;
}

// ヘルパー関数：コマンドキューとリスト作成
HRESULT DXWindow::CreateCommandQueueAndList()
{
    HRESULT hr = S_OK;

    // コマンドキュー作成
    ComPtr<ID3D12CommandQueue> commandQueue;
    hr = CD3DX12CreateCommandQueue(
        GetHWND(),
        &m_commandQueue);

    if (FAILED(hr)) {
        OutputDebugStringA("DXWindow::CreateCommandQueueAndList - CommandQueue creation failed");
        return hr;
    }

    // コマンドリスト作成
    ComPtr<ID3D12GraphicsCommandList4> commandList;
    hr = CD3DX12CreateCommandList(
        GetHWND(),
        &m_commandList);

    if (FAILED(hr)) {
        OutputDebugStringA("DXWindow::CreateCommandQueueAndList - CommandList creation failed");
        return hr;
    }

    // SwapChain 作成
    ComPtr<IDXGISwapChain1> swapChain;
    DXGI_SWAP_CHAIN_DESC scd = {};
    ZeroMemory(&scd, sizeof(scd));

    scd.BufferCount = 2;
    scd.BufferDesc.Width = m_nWidth;
    scd.BufferDesc.Height = m_nHeight;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.RefreshRate.Numerator = 60;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.SampleDesc.Count = 1;
    scd.Windowed = TRUE;

    hr = CD3DX12CreateSwapChain(
        GetHWND(),
        lpfnWndProc,
        (HWND)GetWindowLongPtr(GetHWND()),
        &scd,
        &swapChain);

    if (FAILED(hr)) {
        OutputDebugStringA("DXWindow::CreateCommandQueueAndList - SwapChain creation failed");
        return hr;
    }

    // SwapChain を初期化
    swapChain->Initialize(
        GetHWND(),
        lpfnWndProc,
        (HWND)GetWindowLongPtr(GetHWND()),
        &scd);

    OutputDebugStringA("DXWindow::CreateCommandQueueAndList succeeded");
    return S_OK;
}

// ヘルパー関数：SwapChain 作成
HRESULT DXWindow::CreateSwapChain(HWND hWnd, UINT32 width, UINT32 height)
{
    HRESULT hr = S_OK;

    ComPtr<IDXGISwapChain1> swapChain;
    DXGI_SWAP_CHAIN_DESC scd = {};
    ZeroMemory(&scd, sizeof(scd));

    scd.BufferCount = 2;
    scd.BufferDesc.Width = width;
    scd.BufferDesc.Height = height;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.RefreshRate.Numerator = 60;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.SampleDesc.Count = 1;
    scd.Windowed = TRUE;

    hr = CD3DX12CreateSwapChain(
        hWnd,
        lpfnWndProc,
        (HWND)GetWindowLongPtr(hWnd),
        &scd,
        &swapChain);

    if (FAILED(hr)) {
        OutputDebugStringA("DXWindow::CreateSwapChain failed");
        return hr;
    }

    // SwapChain を初期化
    swapChain->Initialize(
        hWnd,
        lpfnWndProc,
        (HWND)GetWindowLongPtr(hWnd),
        &scd);

    OutputDebugStringA("DXWindow::CreateSwapChain succeeded");
    return S_OK;
}

// ウィンドウプロシージャ
LRESULT CALLBACK DXWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

// メンバ関数のインライン実装
inline HWND DXWindow::GetHWND() const { return m_hWnd; }
inline UINT32 DXWindow::GetWidth() const { return m_nWidth; }
inline UINT32 DXWindow::GetHeight() const { return m_nHeight; }
inline bool DXWindow::IsActive() const { return m_active; }

// リソース管理
void DXWindow::ResetResources()
{
    if (m_swapChain) {
        m_swapChain->Release();
        m_swapChain = nullptr;
    }

    if (m_device) {
        m_device->Release();
        m_device = nullptr;
    }

    if (m_commandQueue) {
        m_commandQueue->Release();
        m_commandQueue = nullptr;
    }

    if (m_commandList) {
        m_commandList->Release();
        m_commandList = nullptr;
    }
}

// リサイズ処理
void DXWindow::ResizeBuffers(int newWidth, int newHeight)
{
    m_nWidth = newWidth;
    m_nHeight = newHeight;

    // SwapChain のバッファをリサイズ
    if (m_swapChain) {
        m_swapChain->ResizeBuffers(
            0,
            newWidth,
            newHeight,
            DXGI_FORMAT_R8G8B8A8_UNORM,
            1);
    }
}
