#include "DeferredComputeShaderClass.h"


DeferredComputeShaderClass::DeferredComputeShaderClass()
{
	mBackBuffer = 0;
}


DeferredComputeShaderClass::~DeferredComputeShaderClass()
{
	if (mBackBuffer)
	{
		mBackBuffer->Release();
		mBackBuffer = 0;
	}
	if (prevRTV)
	{
		for (UINT i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
		{
			prevRTV[i] = 0;
		}
	}
	if (prevDSV)
	{
		prevDSV = 0;
	}
}


bool DeferredComputeShaderClass::Init(ID3D11Device* pDevice, IDXGISwapChain* pSwapChain)
{
	bool result;
	HRESULT hr;
	result = ComputeShaderClass::InitShaders(pDevice, L"data/shaders/DeferredComputeShader.hlsl");
	if (!result)
		return false;
	
	ID3D11Texture2D* backBuffer;
	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	if (FAILED(hr))
	{
		return false;
	}


	hr = pDevice->CreateUnorderedAccessView(backBuffer, NULL, &mBackBuffer);
	if (FAILED(hr))
	{
		return false;
	}
	backBuffer->Release();

	return true;
}


void DeferredComputeShaderClass::Compute(ID3D11DeviceContext* pDeviceContext, DeferredBufferClass* pBufferClass, UINT ThreadGroupCountX, UINT ThreadGroupCountY)
{
	SetShaderParameters(pDeviceContext, pBufferClass);

	ComputeShaderClass::Compute(pDeviceContext, ThreadGroupCountX, ThreadGroupCountY);

	UnsetParameters(pDeviceContext);
}

void DeferredComputeShaderClass::UnsetParameters(ID3D11DeviceContext* pDeviceContext)
{
	ID3D11UnorderedAccessView* uav[] = { nullptr };
	ID3D11ShaderResourceView* srv[BUFFER_COUNT] = { nullptr };
	ID3D11ShaderResourceView* srv2 =nullptr;
	pDeviceContext->CSSetUnorderedAccessViews(0, 1, uav, NULL);
	pDeviceContext->CSSetShaderResources(0, 1, &srv2);
	pDeviceContext->CSSetShaderResources(1, BUFFER_COUNT, srv);

	ID3D11RenderTargetView* nullRTV[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT] = { nullptr };
	ID3D11DepthStencilView* nullDSV = nullptr;
	// Bind the render target view array and depth stencil buffer to the output render pipeline.
	pDeviceContext->OMSetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, nullRTV, nullDSV);
}

bool DeferredComputeShaderClass::SetShaderParameters(ID3D11DeviceContext* pDeviceContext, DeferredBufferClass* pBufferClass)
{
	//pDeviceContext->OMGetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, prevRTV, &prevDSV);
	pDeviceContext->CSSetUnorderedAccessViews(0, 1, &mBackBuffer, NULL);

	ID3D11ShaderResourceView* tex = pBufferClass->GetLightSRV();

	pDeviceContext->CSSetShaderResources(0, 1, &tex);

	tex = pBufferClass->GetShaderResourceView(1);

	// Set shader texture resource in the pixel shader.
	pDeviceContext->CSSetShaderResources(1, 1, &tex);

	return true;
}