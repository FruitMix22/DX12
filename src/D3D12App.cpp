#include "D3D12App.hpp"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

using Microsoft::WRL::ComPtr;

D3D12App::D3D12App(HINSTANCE hInstance, HWND hWnd)
{
	mHInstance = hInstance;
	mHWnd = hWnd;
}

bool D3D12App::Initialize()
{
	InitDirect3D();
	CreateFence();
	CreateCommandObjects();
	CreateSwapChain(); 
	CreateRtvAndDsvDescriptorHeaps();
	CreateRenderViewTarget();
	CreateDepthBufferAndView();
	return true; 
}

bool D3D12App::InitDirect3D()
{
	//**********************//
	//	   Create Device	//
	//**********************//
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

void D3D12App::CreateFence()
{
	//**********************//
	//	   Create Fence	    //
	//**********************//

	ThrowIfFailed(md3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence)));
}

void D3D12App::CreateCommandObjects()
{
	//**********************//
	//	   Create Fence	    //
	//**********************//

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	ThrowIfFailed(md3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mCommandQueue)));

	ThrowIfFailed(md3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(mDirectCmdListAlloc.GetAddressOf())));

	// Create a temporary command list.
	// ID3D12GraphicsCommandList
	//
	// We are using ID3D12GraphicsCommandList6 in our class,
	// so we create the original interface first and then upgrade it
	// using QueryInterface later
	ComPtr<ID3D12GraphicsCommandList> cmdList = nullptr;

	ThrowIfFailed(md3dDevice->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		mDirectCmdListAlloc.Get(), // Associated command allocator
		nullptr,                   // Initial PipelineStateObject
		IID_PPV_ARGS(cmdList.GetAddressOf())));

	// Check if it supports CommandList6, if it does, it swaps it to the new one
	ThrowIfFailed(cmdList->QueryInterface(IID_PPV_ARGS(&mCommandList)));

	// Start off in a closed state.  This is because the first time we refer 
	// to the command list we will Reset it, and it needs to be closed before
	// calling Reset.
	mCommandList->Close();
}

void D3D12App::CreateSwapChain()
{
	// Release the previous swapchain, were recreating it as window size and stuff has changed
	mSwapChain.Reset(); // nullptr

	DXGI_SWAP_CHAIN_DESC1 sd;
	sd.Width = mWindowWidth;
	sd.Height = mWindowHeight;
	sd.Format = mBackBufferFormant;
	sd.Stereo = false;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = mSwapChainBufferCount;
	sd.Scaling = DXGI_SCALING_NONE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain1;
	ThrowIfFailed(mdxgiFactory->CreateSwapChainForHwnd(
		mCommandQueue.Get(), 
		mHWnd, 
		&sd, 
		nullptr, 
		nullptr, 
		swapChain1.GetAddressOf()));

	ThrowIfFailed(swapChain1.As(&mSwapChain));
}

void D3D12App::CreateRtvAndDsvDescriptorHeaps()
{
	mRtvHeap.Init(md3dDevice.Get(),
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV, mSwapChainBufferCount);
	mDsvHeap.Init(md3dDevice.Get(),
		D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1);
}

void D3D12App::CreateRenderViewTarget()
{
	for (UINT i = 0; i < mSwapChainBufferCount; i++)
	{
		ThrowIfFailed(mSwapChain->GetBuffer(i, IID_PPV_ARGS(&mSwapChainBuffer[i])));
		md3dDevice->CreateRenderTargetView(mSwapChainBuffer[i].Get(), nullptr, mRtvHeap.CpuHandle(i));
	}
}

void D3D12App::CreateDepthBufferAndView()
{
	// create depth buffer
	D3D12_RESOURCE_DESC depthDesc = {};
	depthDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthDesc.Alignment = 0; // Fixed typo: Allignment -> Alignment
	depthDesc.Width = mWindowWidth;
	depthDesc.Height = mWindowHeight;
	depthDesc.DepthOrArraySize = 1;
	depthDesc.MipLevels = 1;
	depthDesc.Format = mDepthFormat;
	depthDesc.SampleDesc.Count = 1;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE optClear = {};
	optClear.Format = mDepthFormat;
	optClear.DepthStencil.Depth = 1.0f;
	optClear.DepthStencil.Stencil = 0;

	auto heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	ThrowIfFailed(md3dDevice->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&depthDesc,
		D3D12_RESOURCE_STATE_COMMON,
		&optClear,
		IID_PPV_ARGS(mDepthBuffer.GetAddressOf())));

	// Create descriptor to mip level 0 of entire resource uising the format of the resource
	md3dDevice->CreateDepthStencilView(
		mDepthBuffer.Get(),
		nullptr,
		DepthStencilView());

	// Transition the resource from its initial state to be used as a depth buffer
	CD3DX12_RESOURCE_BARRIER depthBarrier[1];
	depthBarrier[0] = CD3DX12_RESOURCE_BARRIER::Transition(
		mDepthBuffer.Get(),
		D3D12_RESOURCE_STATE_COMMON,
		D3D12_RESOURCE_STATE_DEPTH_WRITE);

	mCommandList->ResourceBarrier(1, depthBarrier);
}

CD3DX12_CPU_DESCRIPTOR_HANDLE D3D12App::CurrentBackBufferView()
{
	return mRtvHeap.CpuHandle(mCurrBackBuffer);
}

CD3DX12_CPU_DESCRIPTOR_HANDLE D3D12App::DepthStencilView()
{
	return mDsvHeap.CpuHandle(0);
}
