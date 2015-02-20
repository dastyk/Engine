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

	bool Init(ID3D11Device* pDevice);

	void Compute(ID3D11DeviceContext* pDeviceContext, ID3D11UnorderedAccessView* pBackBuffer, DeferredBufferClass* pBufferClass, UINT ThreadGroupCountX, UINT ThreadGroupCountY);

private:
	bool SetShaderParameters(ID3D11DeviceContext* pDeviceContext, ID3D11UnorderedAccessView* pBackBuffer, DeferredBufferClass* pBufferClass );

	void UnsetParameters(ID3D11DeviceContext* pDeviceContext);


};

#endif