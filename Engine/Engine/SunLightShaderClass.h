#ifndef _SUNSunLightShaderClass_H_
#define _SUNSunLightShaderClass_H_

#pragma once

#include "ShaderClass.h"


struct LightCBufferType
{
	XMFLOAT3 AmbientColor;
	float pad1;
	XMFLOAT3 diffDir;
	float pad2;
	XMFLOAT3 diffColor;
	float pad3;
	XMFLOAT3 specDir;
	float pad4;
	XMFLOAT3 specColor;
	float specShinyPower;
	XMFLOAT3 fogColor;
	float fogRange;

};

class SunLightShaderClass : public ShaderClass
{
public:
	SunLightShaderClass();
	~SunLightShaderClass();

	bool Init(ID3D11Device* pDevice);
	bool Render(ID3D11DeviceContext* pDeviceContext, int indexCount, XMFLOAT4X4& worldMatrix, XMFLOAT4X4& viewMatrix, XMFLOAT4X4& projMatrix, ID3D11ShaderResourceView* pTexture, XMFLOAT3& ambientColor, XMFLOAT3& diffDir, XMFLOAT3& diffColor, XMFLOAT3& specPos, XMFLOAT3& specColor, float specShinyPower, XMFLOAT3& fogColor, float fogRange);
private:
	bool InitShader(ID3D11Device*, WCHAR*, WCHAR*, WCHAR*);
	bool SetConstantBufferParameters(ID3D11DeviceContext* pDeviceContext, XMFLOAT3& ambientColor, XMFLOAT3& diffDir, XMFLOAT3& diffColor, XMFLOAT3& specDir, XMFLOAT3& specColor, float specShinyPower, XMFLOAT3& fogColor, float fogRange);

	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11SamplerState* mSampleState;

	ID3D11Buffer* mLightBuffer;
};

#endif