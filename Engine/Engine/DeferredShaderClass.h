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

struct CamBuffer
{
	XMFLOAT3 CamPos;
	float pad;
};

struct VPBuffer
{
	XMFLOAT4X4 view;
	XMFLOAT4X4 proj;
};

class DeferredShaderClass :
	public ShaderClass
{
public:
	DeferredShaderClass();
	~DeferredShaderClass();

	bool Init(ID3D11Device* pDevice);
	bool RenderDeferred(ID3D11DeviceContext* pDeviceContext, ObjectClass* pObject, CameraClass* pCamera);
	bool RenderLights(ID3D11DeviceContext* pDeviceContext, CameraClass* pCamera, DeferredBufferClass* pBuffer, PointLightClass** ppLights, UINT NrOfLights);
	bool RenderLights(ID3D11DeviceContext* pDeviceContext, PointLightClass* pLight);



	bool SetLP(ID3D11DeviceContext* pDeviceContext, CameraClass* pCamera, DeferredBufferClass* pBuffer);
	bool UnSetLP(ID3D11DeviceContext* pDeviceContext);
	bool Render(ID3D11DeviceContext* pDeviceContext, DeferredBufferClass* pBuffer);
	
	
	
	//bool RenderShadows(ID3D11DeviceContext* pDeviceContext, DeferredBufferClass* pBuffer, ObjectClass* pObject, CameraClass* pCamera, PointLightClass** ppLights, UINT NrOfLights, FogClass* pDrawDistFog, ID3D11ShaderResourceView* pShadowmap);
private:
	bool InitShader(ID3D11Device*, WCHAR*, WCHAR*, WCHAR*, WCHAR*, WCHAR*, WCHAR*);

	void RenderShader(ID3D11DeviceContext*, int);

	//bool SetConstantBufferParameters(ID3D11DeviceContext* pDeviceContext, PointLightClass** ppLights, UINT NrOfLights, ObjectClass* pObject, FogClass* pDrawDistFog, CameraClass* pCamera);
	bool SetMaterialConstantBufferParameters(ID3D11DeviceContext* pDeviceContext, MatrialDesc* pMaterial);
	bool SetLightConstantBufferParameters(ID3D11DeviceContext* pDeviceContext, PointLightClass* pPointLight);
	bool SetCamConstantBufferParameters(ID3D11DeviceContext* pDeviceContext, CameraClass* pCamera);
	bool SetVPConstantBufferParameters(ID3D11DeviceContext* pDeviceContext, CameraClass* pCamera);

	D3D11_RECT CalcScissorRect(const XMFLOAT3& lightPos, float lightRange, CameraClass* pCamera);
	float Clamp(float in, float v1, float v2)
	{
		in = (in < v1) ? v1 : in;
		in = (in > v2) ? v2 : in;

		return in;
	}


private:
	ID3D11SamplerState* mSampleState;


	ID3D11SamplerState* mSampleStateRenderTarget;
	ID3D11VertexShader* mVertexShaderRenderTarget;
	ID3D11PixelShader* mPixelShaderRenderTarget;
	ID3D11Buffer* mVertexBuffer;
	ID3D11InputLayout* mLayout2;

	ID3D11Buffer* mLightBuffer;
	ID3D11Buffer* mMaterialBuffer;
	ID3D11Buffer* mCamBuffer;
	ID3D11PixelShader* mLightPS;
	ID3D11GeometryShader* mLightGS;
	ID3D11VertexShader* mLightVS;
	ID3D11Buffer* mVPBuffer;

	ID3D11ShaderResourceView* unbindSrv[BUFFER_COUNT];


	ID3D11BlendState* mBlendState;

	float pcolor[4];
	ID3D11BlendState* pBS;
	UINT sampleMask = 0xffffffff;
	ID3D11DepthStencilState* prevDSS;
};

#endif

inline XMFLOAT4 operator+(XMFLOAT4 f1, XMFLOAT4 f2)
{
	XMFLOAT4 out;

	out.x = f1.x + f2.x;
	out.y = f1.y + f2.y;
	out.z = f1.z + f2.z;
	out.w = f1.w + f2.w;

	return out;
}

inline XMFLOAT4 operator-(XMFLOAT4 f1, XMFLOAT4 f2)
{
	XMFLOAT4 out;

	out.x = f1.x - f2.x;
	out.y = f1.y - f2.y;
	out.z = f1.z - f2.z;
	out.w = f1.w - f2.w;

	return out;
}