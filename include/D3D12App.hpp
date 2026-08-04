#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <vector>
#include "d3dUtil.h"

// Link necessary d3d12 libraries.
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

class D3D12App
{
public:
    D3D12App(HINSTANCE hInstance);
    D3D12App(const D3D12App& rhs) = delete;
public:

    static D3D12App* GetApp();
  
    virtual bool Initialize();
  
protected:

    bool InitDirect3D();
    void CreateFence();
    void CreateCommandObjects();

protected:

    Microsoft::WRL::ComPtr<IDXGIFactory6> mdxgiFactory;
    Microsoft::WRL::ComPtr<ID3D12Device5> md3dDevice;

    Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
    UINT64 mCurrentFence = 0;

    // Command queue and command list
    Microsoft::WRL::ComPtr<ID3D12CommandQueue>mCommandQueue;
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator>mDirectCmdListAlloc;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6>mCommandList;
    
};