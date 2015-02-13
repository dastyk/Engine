#ifndef _SHADOWMAPCLASS_H_
#define _SHADOWMAPCLASS_H_

#pragma once
#include "ShaderClass.h"
class ShadowMapClass :
	public ShaderClass
{
public:
	ShadowMapClass();
	~ShadowMapClass();

	bool Init(ID3D11Device* pDevice, float w, float h);
	bool CreateShadowMap(ID3D11DeviceContext* pDeviceContext, ObjectClass* pObject, PointLightClass* pPointLight);

	ID3D11ShaderResourceView* GetShaderResourceView();

private:
	bool InitShader(ID3D11Device*, WCHAR*, WCHAR*, float w, float h);

	void RenderShader(ID3D11DeviceContext*, int);
private: 
	ID3D11RenderTargetView* mRTV;
	ID3D11ShaderResourceView* mSRV;
	ID3D11DepthStencilView* mDSV;
};

#endif