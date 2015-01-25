#ifndef _DEFFEREDBUFFERCLASS_H_
#define _DEFFEREDBUFFERCLASS_H_
#pragma once

const int BUFFER_COUNT = 2;

#include <d3d11.h>

class DefferedBufferClass
{
public:
	DefferedBufferClass();
	~DefferedBufferClass();

	bool Init(ID3D11Device*, int, int);

	void SetRenderTargets(ID3D11DeviceContext*, ID3D11DepthStencilView*);
	void ClearRenderTargets(ID3D11DeviceContext*, float, float, float, float);

	ID3D11ShaderResourceView* GetShaderResourceView(int);
	ID3D11ShaderResourceView** GetShaderResourceView();
private:
	ID3D11Texture2D* mRenderTargetTextureArray[BUFFER_COUNT];
	ID3D11RenderTargetView* mRenderTargetViewArray[BUFFER_COUNT];
	ID3D11ShaderResourceView* mShaderResourceViewArray[BUFFER_COUNT];
};

#endif