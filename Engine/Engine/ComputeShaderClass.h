#ifndef _COMPUTESHADERCLASS_H_
#define _COMPUTESHADERCLASS_H_

#pragma once

#include <fstream>
#include <d3d11.h>
#include <d3dcompiler.h>

#include <DirectXMath.h>
using namespace DirectX;
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")


class ComputeShaderClass
{
public:
	ComputeShaderClass();
	virtual ~ComputeShaderClass();



	virtual bool Init(ID3D11Device* pDevice);

	bool Compute(ID3D11DeviceContext* pDeviceContext, UINT ThreadGroupCountX, UINT ThreadGroupCountY);

protected:
	void OutputShaderErrorMessage(ID3D10Blob*, WCHAR*);

	bool InitShaders(ID3D11Device* pDevice, WCHAR*);

	bool createComputeShader(ID3D11Device* pDevice, WCHAR* cName, char* EntryPoint);
	bool createSamplerState(ID3D11Device* pDevice, ID3D11SamplerState** ppSampleState, D3D11_SAMPLER_DESC* samplerDesc);


	ID3D11ComputeShader* mComputeShader;
	ID3D11SamplerState* mPointSample;


};

#endif