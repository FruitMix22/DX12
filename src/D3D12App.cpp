#include "D3D12App.hpp"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

using Microsoft::WRL::ComPtr;

bool D3D12App::InitDirect3D()
{
	UINT factoryFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)
	factoryFlags = DXGI_CREATE_FACTORY_DEBUG;

	// Enable the D3D12 debug layer
	ComPtr<ID3D12Debug> debugController0;
	ComPtr<ID3D12Debug1> debugController1;
	ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController0)));
	ThrowIfFailed(debugController0->QueryInterface(IID_PPV_ARGS(&debugController1)));
	debugController0->EnableDebugLayer();
	//debugController1->SetEnableGPUBasedValidation(true);
#endif

	ThrowIfFailed(CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&mdxgiFactory)));
}