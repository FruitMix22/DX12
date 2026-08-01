#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include "d3dUtil.h"

// Link necessary d3d12 libraries.
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxcompiler.lib") // dxc

class D3D12App
{
protected:
    D3D12App(HINSTANCE hInstance);
    D3D12App(const D3D12App& rhs) = delete;
    D3D12App& operator=(const D3D12App& rhs) = delete;
    virtual ~D3D12App();

public:

    static D3D12App* GetApp();
  
    virtual bool Initialize();
  
protected:

    bool InitDirect3D();

protected:

    Microsoft::WRL::ComPtr<IDXGIFactory6> mdxgiFactory;
    
};