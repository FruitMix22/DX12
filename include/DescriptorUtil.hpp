#include <d3d12.h>
#include <wrl.h>
#include "d3dx12.h"

class DescriptorHeap
{
public:
	DescriptorHeap() = default;
	DescriptorHeap(const DescriptorHeap& rhs) = delete;
	DescriptorHeap& operator=(const DescriptorHeap& rhs) = delete;

	void Init(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT capacity);

	ID3D12DescriptorHeap* GetD3dHeap() const;
	
	CD3DX12_CPU_DESCRIPTOR_HANDLE CpuHandle(uint32_t index);
	CD3DX12_GPU_DESCRIPTOR_HANDLE GpuHandle(uint32_t index);

protected:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mHeap = nullptr;
	UINT mDescriptorHeap = 0;
};