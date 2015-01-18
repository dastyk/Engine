#ifndef _PARTICLESHADERCLASS_H_
#define _PARTICLESHADERCLASS_H_

#pragma once



#include "ShaderClass.h"

struct ParticleConstBufferType
{
	XMFLOAT4X4 Proj;
	XMFLOAT4X4 View;
};
class ParticleShaderClass :
	public ShaderClass
{
public:
	ParticleShaderClass();
	~ParticleShaderClass();

	bool Init(ID3D11Device* pDevice);
	bool Render(ID3D11DeviceContext* pDeviceContext, int vertexCount, CameraClass* pCamera);
	

private:
	bool InitShader(ID3D11Device*, WCHAR*, WCHAR*, WCHAR*);
	bool SetConstantBufferParameters(ID3D11DeviceContext* pDeviceContext, XMFLOAT4X4& viewMatrix, XMFLOAT4X4& projMatrix);

	void RenderShader(ID3D11DeviceContext*, int);
};

#endif