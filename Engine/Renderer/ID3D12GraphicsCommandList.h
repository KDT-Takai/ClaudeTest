#pragma once

// DirectX 12 Graphics Command List Forward Declaration
// This provides a forward declaration for use with CD3DX12 helper functions

class ID3D12GraphicsCommandList;

// Helper macro to create and initialize command lists
#define CD3DX12CreateCommandList(hInstance, ...) \
    (CD3DX12CreateCommandListEx(hInstance, __VA_ARGS__))

// Helper macro to create and initialize swap chains
#define CD3DX12CreateSwapChain(hInstance, lpfnWndProc, hWnd, desc, pSwapChain) \
    (CD3DX12CreateSwapChainEx(hInstance, lpfnWndProc, hWnd, desc, pSwapChain))

// Helper macro to create and initialize D3D12 devices
#define CD3DX12CreateDevice(hInstance, featureLevels, ppFeatureLevelData) \
    (CD3DX12CreateDeviceEx(hInstance, featureLevels, ppFeatureLevelData))

// Helper macro to create and initialize command queues
#define CD3DX12CreateCommandQueue(hInstance, desc) \
    (CD3DX12CreateCommandQueueEx(hInstance, desc))
