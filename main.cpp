#include <windows.h>
#include "Engine/Window/Window.h"
#include "Engine/DirectX12/DirectX12Window.h"

// 簡易的な描画関数
void SimpleRender()
{
    // ここに DirectX12 の描画コードを書く
    // 例：クリアコマンド、頂点セットアップなど
    
    ID3D12GraphicsCommandList* cmdList = m_pSwapChain->GetCommandQueue()->GetCommandList();
    
    // バックバッファのクリア
    CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM) clearValue;
    clearValue.Color[0].r = 0.5f;  // 半透明の青
    clearValue.Color[0].g = 0.7f;
    clearValue.Color[0].b = 1.0f;
    clearValue.Color[0].a = 1.0f;
    
    cmdList->ClearRenderTargetView(
        m_pSwapChain->GetBackBuffer(0, 0),
        &clearValue);
    
    // 描画コマンドの実行
    cmdList->Close();
}

int main()
{
    // DXWindow の作成と初期化
    HWND hWnd = GetConsoleWindow();  // コンソールウィンドウを使用
    DXWindow window(hWnd, 800, 600, L"DirectX12 Test Window");
    
    if (!window.Create()) {
        MessageBoxA(NULL, "ウィンドウの作成に失敗しました", "エラー", MB_ICONERROR);
        return 1;
    }

    // コンテキストの作成
    if (!window.CreateContext()) {
        MessageBoxA(NULL, "DirectX12 コンテキストの作成に失敗しました", "エラー", MB_ICONERROR);
        return 1;
    }

    // メッセージループと描画ループ
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        
        // 簡易的な描画処理（実際には SwapChain のバッファをクリア）
        SimpleRender();
    }

    return 0;
}