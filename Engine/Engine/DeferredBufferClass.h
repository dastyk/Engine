#ifndef _DEFERREDBUFFERCLASS_H_
#define _DEFERREDBUFFERCLASS_H_
#pragma once

const int BUFFER_COUNT = 3;

#include <d3d11.h>

class DeferredBufferClass
{
public:
	DeferredBufferClass();
	~DeferredBufferClass();

	bool Init(ID3D11Device*, int, int);

	void SetRenderTargets(ID3D11DeviceContext*);
	void UnsetRenderTargets(ID3D11DeviceContext*);
	void ClearRenderTargets(ID3D11DeviceContext*, float, float, float, float);

	ID3D11ShaderResourceView* GetShaderResourceView(int);
	ID3D11ShaderResourceView** GetShaderResourceView();
private:
	ID3D11Texture2D* mRenderTargetTextureArray[BUFFER_COUNT];
	ID3D11RenderTargetView* mRenderTargetViewArray[BUFFER_COUNT];
	ID3D11ShaderResourceView* mShaderResourceViewArray[BUFFER_COUNT];
	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11DepthStencilView* mDepthStencilView;
	D3D11_VIEWPORT mViewport;

	ID3D11DepthStencilView* prevDSV = nullptr;
	ID3D11RenderTargetView* prevRTV[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
};

#endif