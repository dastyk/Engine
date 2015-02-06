#ifndef _LIGHTSHADERCLASS_H_
#define _LIGHTSHADERCLASS_H_

#pragma once

#include "ShaderClass.h"

struct LMatrixBufferType
{
	XMFLOAT4X4 mWorldViewProj;
	XMFLOAT4X4 mWorld;
	XMFLOAT4X4 mWorldView;
	XMFLOAT4X4 mBones[MAX_BONE_COUNT];
};

class LightShaderClass : public ShaderClass
{
public:
	LightShaderClass();
	~LightShaderClass();

	bool Init(ID3D11Device* pDevice);
	bool Render(ID3D11DeviceContext* pDeviceContext, ObjectClass* pObject, CameraClass* pCamera, PointLightClass** ppLights, UINT NrOfLights, FogClass* pDrawDistFog);
	bool RenderAnimated(ID3D11DeviceContext* pDeviceContext, ObjectClass* pObject, CameraClass* pCamera, PointLightClass** ppLights, UINT NrOfLights, FogClass* pDrawDistFog);

private:
	bool InitShader(ID3D11Device*, WCHAR*, WCHAR*, WCHAR*);
	bool SetConstantBufferParameters(ID3D11DeviceContext* pDeviceContext, PointLightClass** ppLights, UINT NrOfLights, ObjectClass* pObject, FogClass* pDrawDistFog, CameraClass* pCamera);
	bool SetTbufferParameters(ID3D11DeviceContext* pDeviceContext, XMFLOAT4X4& worldMatrix, XMFLOAT4X4& viewMatrix, XMFLOAT4X4& projMatrix, ObjectClass* pObject);


	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11SamplerState* mSampleState;

	ID3D11Buffer* mLightBuffer;
	ID3D11Buffer* mBoneTBuffer;
	ID3D11ShaderResourceView* mBoneSRV;

	ID3D11GeometryShader* mAniGS;

};

#endif