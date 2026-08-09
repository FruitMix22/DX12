#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <vector>
#include "d3dUtil.h"
#include "DescriptorUtil.hpp"

// Link necessary d3d12 libraries.
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

class D3D12App
{
public:
    D3D12App(HINSTANCE hInstance, HWND hWnd);
    D3D12App(const D3D12App& rhs) = delete;
public:

    static D3D12App* GetApp();
  
    virtual bool Initialize();
  
protected:

private:
    HINSTANCE mHInstance;
    HWND mHWnd;

    bool InitDirect3D();
    void CreateFence();
    void CreateCommandObjects();
    void CreateSwapChain();
    void CreateRtvAndDsvDescriptorHeaps();

public:
    CD3DX12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView();
    CD3DX12_CPU_DESCRIPTOR_HANDLE DepthStencilView();

protected:

    Microsoft::WRL::ComPtr<IDXGIFactory6> mdxgiFactory;
    Microsoft::WRL::ComPtr<ID3D12Device5> md3dDevice;

    Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
    UINT64 mCurrentFence = 0;

    // Command queue and command list
    Microsoft::WRL::ComPtr<ID3D12CommandQueue>mCommandQueue;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator>mDirectCmdListAlloc;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6>mCommandList;

    // Swap chain
    typedef struct DXGI_SWAP_CHAIN_DESC
    {
        DXGI_MODE_DESC BufferDesc;
        DXGI_SAMPLE_DESC SampleDesc;  
        DXGI_USAGE BufferUsage;
        UINT BufferCount;
        HWND OutputWindow;
        BOOL Windowed;
        DXGI_SWAP_EFFECT SwapEffect;
        UINT Flags;
    }   DXGI_SWAP_CHAIN_DESC;
    
    typedef struct DXGI_MODE_DESC
    {
        UINT Width;
        UINT Height;
        DXGI_RATIONAL RefreshRate;
        DXGI_MODE_SCANLINE_ORDER ScanlineOrdering;
        DXGI_MODE_SCALING Scaling;
    }   DXGI_MODE_DESC;

    DXGI_FORMAT mBackBufferFormant = DXGI_FORMAT_R8G8B8A8_UNORM;
    Microsoft::WRL::ComPtr<IDXGISwapChain4> mSwapChain;
    static const int mSwapChainBufferCount = 2;
    int mCurrBackBuffer = 0;
    UINT mWindowWidth = 1280;
    UINT mWindowHeight = 720;

    // Descriptor
    DescriptorHeap mRtvHeap;
    DescriptorHeap mDsvHeap;


};