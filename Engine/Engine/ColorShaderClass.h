#ifndef _COLORSHADERCLASS_H_
#define _COLORSHADERCLASS_H_

#pragma once

#include "ShaderClass.h"



class ColorShaderClass : public ShaderClass
{
public:
	ColorShaderClass();
	~ColorShaderClass();

	bool Init(ID3D11Device* pDevice);
	bool Render(ID3D11DeviceContext* pDeviceContext, int indexCount, XMFLOAT4X4& worldMatrix, XMFLOAT4X4& viewMatrix, XMFLOAT4X4& projMatrix);
private:
	bool InitShader(ID3D11Device*, WCHAR*, WCHAR*);

	void RenderShader(ID3D11DeviceContext*, int);

private:
	
};

#endif