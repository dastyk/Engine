#include "DeferredComputeShaderClass.h"


DeferredComputeShaderClass::DeferredComputeShaderClass()
{
}


DeferredComputeShaderClass::~DeferredComputeShaderClass()
{
}


bool DeferredComputeShaderClass::Init(ID3D11Device* pDevice)
{
	bool result;
	result = InitShaders(pDevice, L"data/shaders/DeferredComputeShader.hlsl");
	if (!result)
		return false;
	

	return true;
}


void DeferredComputeShaderClass::Compute(ID3D11DeviceContext* pDeviceContext, ID3D11UnorderedAccessView* pBackBuffer, DeferredBufferClass* pBufferClass, UINT ThreadGroupCountX, UINT ThreadGroupCountY)
{
	SetShaderParameters(pDeviceContext, pBackBuffer, pBufferClass);

	ComputeShaderClass::Compute(pDeviceContext, ThreadGroupCountX, ThreadGroupCountY);

	UnsetParameters(pDeviceContext);
}

void DeferredComputeShaderClass::UnsetParameters(ID3D11DeviceContext* pDeviceContext)
{
	ID3D11UnorderedAccessView* uav[] = { nullptr };
	pDeviceContext->CSSetUnorderedAccessViews(0, 1, uav, NULL);
}

bool DeferredComputeShaderClass::SetShaderParameters(ID3D11DeviceContext* pDeviceContext, ID3D11UnorderedAccessView* pBackBuffer, DeferredBufferClass* pBufferClass)
{
	pDeviceContext->CSSetUnorderedAccessViews(0, 1, &pBackBuffer, NULL);

	return true;
}