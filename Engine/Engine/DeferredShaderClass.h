#ifndef _DEFERREDSHADERCLASS_H_
#define _DEFERREDSHADERCLASS_H_
#pragma once

#include "ShaderClass.h"
#include "DeferredBufferClass.h"

struct DeferredVertexStruct
{
	XMFLOAT3 Pos;
	XMFLOAT2 Tex;
};

struct ShadowBuffer
{
	XMFLOAT4X4 LightViewProj;
	XMFLOAT3 LightPos;
	float pad;
};


class DeferredShaderClass :
	public ShaderClass
{
public:
	DeferredShaderClass();
	~DeferredShaderClass();

	bool Init(ID3D11Device* pDevice);
	bool RenderDeferred(ID3D11DeviceContext* pDeviceContext, ObjectClass* pObject, CameraClass* pCamera);
	bool Render(ID3D11DeviceContext* pDeviceContext, DeferredBufferClass* pBuffer, ObjectClass* pObject, CameraClass* pCamera, PointLightClass** ppLights, UINT NrOfLights, FogClass* pDrawDistFog);
	bool RenderShadows(ID3D11DeviceContext* pDeviceContext, DeferredBufferClass* pBuffer, ObjectClass* pObject, CameraClass* pCamera, PointLightClass** ppLights, UINT NrOfLights, FogClass* pDrawDistFog, ID3D11ShaderResourceView* pShadowmap);
private:
	bool InitShader(ID3D11Device*, WCHAR*, WCHAR*, WCHAR*, WCHAR*, WCHAR*, WCHAR*);

	void RenderShader(ID3D11DeviceContext*, int);

	bool SetConstantBufferParameters(ID3D11DeviceContext* pDeviceContext, PointLightClass** ppLights, UINT NrOfLights, ObjectClass* pObject, FogClass* pDrawDistFog, CameraClass* pCamera);
	bool SetShadowConstantBufferParamters(ID3D11DeviceContext* pDeviceContext, PointLightClass* pPointLight);

private:
	ID3D11SamplerState* mSampleState;


	ID3D11SamplerState* mSampleStateRenderTarget;
	ID3D11VertexShader* mVertexShaderRenderTarget;
	ID3D11PixelShader* mPixelShaderRenderTarget;
	ID3D11Buffer* mVertexBuffer;
	ID3D11InputLayout* mLayout2;

	ID3D11Buffer* mLightBuffer;

	ID3D11Buffer* mShadowBuffer;
	ID3D11VertexShader* mShadowDeferredVS;
	ID3D11PixelShader* mShadowDeferredPS;

	ID3D11ShaderResourceView* unbindSrv[BUFFER_COUNT];
};

#endif