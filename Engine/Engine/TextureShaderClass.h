#ifndef _TEXTURESHADERCLASS_H_
#define _TEXTURESHADERCLASS_H_

#pragma once

#include "ShaderClass.h"


class TextureShaderClass : public ShaderClass
{
public:
	TextureShaderClass();
	~TextureShaderClass();

	bool Init(ID3D11Device* pDevice);
	bool Render(ID3D11DeviceContext* pDeviceContext, int indexCount, XMFLOAT4X4& worldMatrix, XMFLOAT4X4& viewMatrix, XMFLOAT4X4& projMatrix, ID3D11ShaderResourceView* pTexture);
private:
	bool InitShader(ID3D11Device*, WCHAR*, WCHAR*, WCHAR*);


	void RenderShader(ID3D11DeviceContext*, int);


private:
	ID3D11SamplerState* mSampleState;

	ID3D11ShaderResourceView* mNullSRW = nullptr;
};

#endif