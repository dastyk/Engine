#ifndef _DEFFEREDSHADERCLASS_H_
#define _DEFFEREDSHADERCLASS_H_
#pragma once
#include "ShaderClass.h"
#include "DefferedBufferClass.h"

class DefferedShaderClass :
	public ShaderClass
{
public:
	DefferedShaderClass();
	~DefferedShaderClass();

	bool Init(ID3D11Device* pDevice);
	bool RenderDeffered(ID3D11DeviceContext* pDeviceContext, ObjectClass* pObject, CameraClass* pCamera);
	bool Render(ID3D11DeviceContext* pDeviceContext, DefferedBufferClass* pBuffer);

private:
	bool InitShader(ID3D11Device*, WCHAR*, WCHAR*, WCHAR*, WCHAR*, WCHAR*, WCHAR*);

	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11SamplerState* mSampleState;

	ID3D11SamplerState* mSampleStateRenderTarget;

	ID3D11VertexShader* mVertexShaderRenderTarget;
	ID3D11PixelShader* mPixelShaderRenderTarget;
	ID3D11GeometryShader* mGeometryShaderRenderTarget;
};

#endif