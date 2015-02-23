#ifndef _SHADOWMAPCLASS_H_
#define _SHADOWMAPCLASS_H_

#pragma once

#define SHADOW_DETAIL 4

#include "ShaderClass.h"
class ShadowMapClass :
	public ShaderClass
{
public:
	ShadowMapClass();
	~ShadowMapClass();

	bool Init(ID3D11Device* pDevice, float w, float h);
	bool CreateShadowMap(ID3D11DeviceContext* pDeviceContext, ObjectClass* pObject, PointLightClass* pPointLight);
	void SetRTV(ID3D11DeviceContext* pDeviceContext);
	void ClearRTV(ID3D11DeviceContext* pDeviceContext);
	void UnbindRTV(ID3D11DeviceContext* pDeviceContext);

	ID3D11ShaderResourceView* GetShaderResourceView();

private:
	bool InitShader(ID3D11Device*, WCHAR*, WCHAR*, WCHAR*, float w, float h);

	void RenderShader(ID3D11DeviceContext*, int);
private: 
	ID3D11RenderTargetView* mRTV;
	ID3D11ShaderResourceView* mSRV;
	ID3D11DepthStencilView* mDSV;
	D3D11_VIEWPORT mViewport;

	ID3D11RenderTargetView* prevRTV[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
	ID3D11DepthStencilView* prevDSV = nullptr;
	D3D11_VIEWPORT prevVP;
};

#endif