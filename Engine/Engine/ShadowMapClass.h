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

	bool Init(ID3D11Device* pDevice);
	bool CreateShadowMap(ID3D11DeviceContext* pDeviceContext, ObjectClass* pObject, CameraClass* pCamera);

private:
	bool InitShader(ID3D11Device*, WCHAR*);

	void RenderShader(ID3D11DeviceContext*, int);

	bool SetConstantBufferParameters(ID3D11DeviceContext* pDeviceContext, PointLightClass** ppLights, UINT NrOfLights, ObjectClass* pObject, FogClass* pDrawDistFog, CameraClass* pCamera);

private:
	ID3D11SamplerState* mSampleState;


	ID3D11SamplerState* mSampleStateRenderTarget;

	ID3D11Buffer* mLightBuffer;

	ID3D11ShaderResourceView* unbindSrv;

};

#endif