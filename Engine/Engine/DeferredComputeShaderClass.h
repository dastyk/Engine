#ifndef _DEFERREDCOMPUTESHADERCLASS_H_
#define _DEFERREDCOMPUTESHADERCLASS_H_


#pragma once
#include "ComputeShaderClass.h"
#include "DeferredBufferClass.h"

class DeferredComputeShaderClass :
	public ComputeShaderClass
{
public:
	DeferredComputeShaderClass();
	~DeferredComputeShaderClass();

	bool Init(ID3D11Device* pDevice, IDXGISwapChain* pSwapChain);

	void Compute(ID3D11DeviceContext* pDeviceContext, DeferredBufferClass* pBufferClass, UINT ThreadGroupCountX, UINT ThreadGroupCountY);

private:
	bool SetShaderParameters(ID3D11DeviceContext* pDeviceContext, DeferredBufferClass* pBufferClass );

	void UnsetParameters(ID3D11DeviceContext* pDeviceContext);

	ID3D11UnorderedAccessView* mBackBuffer;

};

#endif