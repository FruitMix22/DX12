#include "D3D12App.hpp"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

using Microsoft::WRL::ComPtr;

D3D12App::D3D12App(HINSTANCE hInstance)
{

}

bool D3D12App::Initialize()
{
	if (!InitDirect3D())
	{
		return false;
	}

	return true;
}

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

	std::vector<ComPtr<IDXGIAdapter>> adapters;
	ComPtr<IDXGIAdapter> foundAdapter;

	// Find an adapter that supports D3D_FEATURE_LEVEL_12_2
		// This is mainly for laptops so it picks GPU over IGPU
	HRESULT hardwareResult = E_FAIL;

	for (int i = 0; mdxgiFactory->EnumAdapters(i, &foundAdapter) != DXGI_ERROR_NOT_FOUND; i++)
	{
		// Try to create hardware device
		ComPtr<ID3D12Device> device = nullptr;
		hardwareResult = D3D12CreateDevice(
			foundAdapter.Get(), 
			D3D_FEATURE_LEVEL_12_2, 
			IID_PPV_ARGS(&device));

		if (SUCCEEDED(hardwareResult))
		{
			ThrowIfFailed(device->QueryInterface(IID_PPV_ARGS(&md3dDevice)));
			break;
		}
	}
}