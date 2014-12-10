#ifndef _TERRAINSHADERCLASS_H_
#define _TERRAINSHADERCLASS_H_

#pragma once

#include "ShaderClass.h"


struct TerrainCBufferType
{
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
	float specShinyPower;

	XMFLOAT3 fogColor;
	float fogRange;
};

struct TextureInfoBufferType
{
	int textureCount;
	XMFLOAT3 pad1;
	bool useBlendMap;
	XMFLOAT3 pad5;
};

class TerrainShaderClass : public ShaderClass
{
public:
	TerrainShaderClass();
	~TerrainShaderClass();

	bool Init(ID3D11Device* pDevice);
	bool Render(ID3D11DeviceContext* pDeviceContext, ObjectClass* pObject, CameraClass* pCamera, LightObjectClass* pSunLightObject, MaterialClass* pMaterial, FogClass* pDrawDistFog);

private:
	bool InitShader(ID3D11Device*, WCHAR*, WCHAR*, WCHAR*);
	bool SetConstantBufferParameters(ID3D11DeviceContext* pDeviceContext, LightObjectClass* pSunLightObject, MaterialClass* pMaterial, FogClass* pDrawDistFog);
	bool SetTextureConstantBufferParamters(ID3D11DeviceContext* pDeviceContext, TextureClass* pTexture);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11SamplerState* mSampleState;

	ID3D11Buffer* mLightBuffer;
	ID3D11Buffer* mTextureInfoBuffer;
};

#endif