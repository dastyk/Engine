#ifndef _DEFERREDSHADERCLASS_H_
#define _DEFERREDSHADERCLASS_H_
#pragma once

#define MAX_ACTIVE_LIGHTS 10

#include "ShaderClass.h"
#include "DeferredBufferClass.h"

#include "TextureClass.h"

struct DeferredVertexStruct
{
	XMFLOAT3 Pos;
	XMFLOAT2 Tex;
};

struct PointLight
{
	XMFLOAT3 Pos;
	float pad1;

	XMFLOAT3 ambientColor;
	float pad2;
	
	XMFLOAT3 diffColor;
	float pad4;
	
	XMFLOAT3 specColor;
	float pad6;
	
	XMFLOAT4 LightRange_Unused_Unused_Unused;
};

struct DeferredLightConstantBuffer
{
	XMFLOAT4 LightCount_FogRange_ShinyPower_Unused;

	XMFLOAT3 ambientReflection;
	float pad3;

	XMFLOAT3 diffReflection;
	float pad5;

	XMFLOAT3 specReflection;
	float pad7;

	XMFLOAT3 fogColor;
	float pad1;

	XMFLOAT3 CamPos;
	float pad2;

	PointLight lights[MAX_ACTIVE_LIGHTS];
};


class DeferredShaderClass :
	public ShaderClass
{
public:
	DeferredShaderClass();
	~DeferredShaderClass();

	bool Init(ID3D11Device* pDevice);
	bool RenderDeferred(ID3D11DeviceContext* pDeviceContext, ObjectClass* pObject, CameraClass* pCamera);
	bool Render(ID3D11DeviceContext* pDeviceContext, DeferredBufferClass* pBuffer);

private:
	bool InitShader(ID3D11Device*, WCHAR*, WCHAR*, WCHAR*, WCHAR*, WCHAR*, WCHAR*);

	void RenderShader(ID3D11DeviceContext*, int);

	//bool SetConstantBufferParameters(ID3D11DeviceContext* pDeviceContext, CameraClass* pCamera, PointLightClass** ppLights, int LightCount, MaterialClass* pMaterial, FogClass* pDrawDistFog);

private:
	ID3D11SamplerState* mSampleState;

	ID3D11SamplerState* mSampleStateRenderTarget;

	ID3D11VertexShader* mVertexShaderRenderTarget;
	ID3D11PixelShader* mPixelShaderRenderTarget;
	ID3D11InputLayout* mLayout;

	ID3D11Buffer* mVertexBuffer;

	TextureClass* mTexture;

	ID3D11Buffer* mLightBuffer;
};

#endif