#ifndef _DEFERREDBUFFERCLASS_H_
#define _DEFERREDBUFFERCLASS_H_
#pragma once

#define BUFFER_COUNT 4

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
	void ClearDepthBuffer(ID3D11DeviceContext*);

	void SetLightRT(ID3D11DeviceContext*);


	ID3D11ShaderResourceView* GetShaderResourceView(int);
	ID3D11ShaderResourceView** GetShaderResourceView();
	ID3D11ShaderResourceView* GetLightSRV();

private:
	ID3D11Texture2D* mRenderTargetTextureArray[BUFFER_COUNT];
	ID3D11RenderTargetView* mRenderTargetViewArray[BUFFER_COUNT];
	ID3D11ShaderResourceView* mShaderResourceViewArray[BUFFER_COUNT];
	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11DepthStencilView* mDepthStencilView;
	D3D11_VIEWPORT mViewport;

	ID3D11Texture2D* mLightTexture;
	ID3D11RenderTargetView* mLightRTV;
	ID3D11ShaderResourceView* mLightSRV;
};

#endif