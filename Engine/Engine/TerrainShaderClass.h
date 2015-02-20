#ifndef _TERRAINSHADERCLASS_H_
#define _TERRAINSHADERCLASS_H_

#pragma once

#include "ShaderClass.h"
#include "MaterialClass.h"

struct TerrainCBufferType
{
	XMFLOAT4 TCBMSPFR;
	XMFLOAT3 ambientColor;
	float pad1;
	XMFLOAT3 ambientReflection;
	float pad2;

	XMFLOAT3 diffDir;
	float pad3;
	XMFLOAT3 diffColor;
	float pad4;
	XMFLOAT3 diffReflection;
	float pad5;

	XMFLOAT3 specDir;
	float pad6;
	XMFLOAT3 specColor;
	float pad7;
	XMFLOAT3 specReflection;
	float pad8;

	XMFLOAT3 fogColor;
	float pad9;
};

struct ShadowBuffer
{
	XMFLOAT4X4 worldViewProj;
	XMFLOAT4X4 world;
	XMFLOAT4X4 LightWorldViewProj;
	XMFLOAT4 camPos;
};

class TerrainShaderClass : public ShaderClass
{
public:
	TerrainShaderClass();
	~TerrainShaderClass();

	bool Init(ID3D11Device* pDevice);
	bool Render(ID3D11DeviceContext* pDeviceContext, ObjectClass* pObject, CameraClass* pCamera, LightObjectClass* pSunLightObject, FogClass* pDrawDistFog);
	bool RenderDeferred(ID3D11DeviceContext* pDeviceContext, ObjectClass* pObject, CameraClass* pCamera);
	bool RenderShadowsDeferred(ID3D11DeviceContext* pDeviceContext, ObjectClass* pObject, CameraClass* pCamera, PointLightClass* pLights, ID3D11ShaderResourceView* pShadowmap);
	bool RenderShadowsDeferred(ID3D11DeviceContext* pDeviceContext, ObjectClass* pObject, CameraClass* pCamera, PointLightClass* pLights, ID3D11ShaderResourceView* pShadowmap, UINT indexCount, UINT indexStart);

private:
	bool InitShader(ID3D11Device*, WCHAR*, WCHAR*, WCHAR*);
	bool SetConstantBufferParameters(ID3D11DeviceContext* pDeviceContext, LightObjectClass* pSunLightObject, FogClass* pDrawDistFog);
	bool SetTextureConstantBufferParamters(ID3D11DeviceContext* pDeviceContext, TextureClass* pTexture);
	void RenderShader(ID3D11DeviceContext*, int);
	bool SetShadowConstantBufferParamters(ID3D11DeviceContext* pDeviceContext, ObjectClass* pObject, CameraClass* pCamera, PointLightClass* pPointLight);

private:
	ID3D11SamplerState* mSampleState;

	ID3D11Buffer* mLightBuffer;
	ID3D11PixelShader* mDeferredPS;


	ID3D11Buffer* mShadowBuffer;
	ID3D11VertexShader* mShadowDeferredVS;
	ID3D11PixelShader* mShadowDeferredPS;
	ID3D11GeometryShader* mShadowDeferredGS;
	ID3D11SamplerState* mPointSampleState;
};

#endif