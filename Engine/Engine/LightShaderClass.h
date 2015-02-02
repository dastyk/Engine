#ifndef _LIGHTSHADERCLASS_H_
#define _LIGHTSHADERCLASS_H_

#pragma once

#define MAX_ACTIVE_LIGHTS 10
#define MAX_MATERIAL_COUNT 10

#include "ShaderClass.h"

struct PointLight
{
	XMFLOAT3 Pos;
	float pad1;

	XMFLOAT4 Color_LightRange;
};

struct LightConstantBuffer
{
	XMFLOAT4 LightCount_FogRange_ObjectCount_Unused;

	XMFLOAT3 fogColor;
	float pad1;

	XMFLOAT3 CamPos;
	float pad2;

	PointLight lights[MAX_ACTIVE_LIGHTS];
	MatrialDescPadded materials[MAX_MATERIAL_COUNT];
};

class LightShaderClass : public ShaderClass
{
public:
	LightShaderClass();
	~LightShaderClass();

	bool Init(ID3D11Device* pDevice);
	bool Render(ID3D11DeviceContext* pDeviceContext, ObjectClass* pObject, CameraClass* pCamera, PointLightClass** ppLights, UINT NrOfLights, FogClass* pDrawDistFog);

private:
	bool InitShader(ID3D11Device*, WCHAR*, WCHAR*, WCHAR*);
	bool SetConstantBufferParameters(ID3D11DeviceContext* pDeviceContext, PointLightClass** ppLights, UINT NrOfLights, ObjectClass* pObject, FogClass* pDrawDistFog, CameraClass* pCamera);

	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11SamplerState* mSampleState;

	ID3D11Buffer* mLightBuffer;
};

#endif