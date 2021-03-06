#ifndef _SHADERCLASS_H_
#define _SHADERCLASS_H_

#define MAX_ACTIVE_LIGHTS 10
#define MAX_MATERIAL_COUNT 10
#define MAX_BONE_COUNT 100
#pragma once

#include <fstream>

#include <d3d11.h>
#include <d3dcompiler.h>

#include <DirectXMath.h>
using namespace DirectX;
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#include "LightObjectClass.h"
#include "FogClass.h"
#include "ObjectClass.h"
#include "CameraClass.h"
#include "ParticleSystemClass.h"
#include "PointLightClass.h"

struct PointLight
{
	XMFLOAT3 Pos;
	float pad1;

	XMFLOAT4 Color_LightRange;
};



struct LightConstantBuffer
{
	XMFLOAT4 LightCount_FogRange_ObjectCount_Unused;

	XMFLOAT3 fogColor;
	float pad1;

	XMFLOAT3 CamPos;
	float pad2;

	PointLight lights[MAX_ACTIVE_LIGHTS];
	MatrialDescPadded materials[MAX_MATERIAL_COUNT];
};

struct MatrixBufferType
{
	XMFLOAT4X4 mWorldViewProj;
	XMFLOAT4X4 mWorld;
	XMFLOAT4 camPos;
};


class ShaderClass
{
public:
	ShaderClass();
	virtual ~ShaderClass();

	virtual bool Init(ID3D11Device* pDevice);
	virtual bool Render(ID3D11DeviceContext* pDeviceContext, ParticleSystemClass* mParticleS, CameraClass* pCamera);
	virtual bool Render(ID3D11DeviceContext* pDeviceContext, int indexCount, XMFLOAT4X4& worldMatrix, XMFLOAT4X4& viewMatrix, XMFLOAT4X4& projMatrix);
	virtual bool Render(ID3D11DeviceContext* pDeviceContext, int indexCount, XMFLOAT4X4& worldMatrix, XMFLOAT4X4& viewMatrix, XMFLOAT4X4& projMatrix, ID3D11ShaderResourceView* pTexture);
	virtual bool Render(ID3D11DeviceContext* pDeviceContext, ObjectClass* pObject, CameraClass* pCamera, PointLightClass** ppLights, UINT NrOfLights, FogClass* pDrawDistFog);

protected:
	void OutputShaderErrorMessage(ID3D10Blob*, WCHAR*);

	bool SetConstantBufferParameters(ID3D11DeviceContext* pDeviceContext, XMFLOAT4X4& worldMatrix, XMFLOAT4X4& viewMatrix, XMFLOAT4X4& projMatrix, XMFLOAT3& direction);

	bool createVertexShaderAndInputLayout(ID3D11Device* pDevice, WCHAR* fileName, CHAR* EntryPoint, D3D11_INPUT_ELEMENT_DESC *vertexDesc, int numElements);
	bool createPixelShader(ID3D11Device* pDevice, WCHAR* fileName, CHAR* EntryPoint);
	bool createGeometryShader(ID3D11Device* pDevice, WCHAR* fileName, CHAR* EntryPoint);
	bool createInputLayout(ID3D11Device* pDevice, D3D11_INPUT_ELEMENT_DESC *vertexDesc, ID3D10Blob* pVertexShaderBuffer, int numElements);

	bool createVertexShaderAndInputLayout(ID3D11Device* pDevice, WCHAR* fileName, CHAR* EntryPoint, D3D11_INPUT_ELEMENT_DESC *vertexDesc, int numElements, ID3D11VertexShader**, ID3D11InputLayout** );
	bool createVertexShader(ID3D11Device* pDevice, WCHAR* fileName, CHAR* EntryPoint, ID3D11VertexShader**);
	bool createPixelShader(ID3D11Device* pDevice, WCHAR* fileName, CHAR* EntryPoint, ID3D11PixelShader**);
	bool createGeometryShader(ID3D11Device* pDevice, WCHAR* fileName, CHAR* EntryPoint, ID3D11GeometryShader**);
	bool createInputLayout(ID3D11Device* pDevice, D3D11_INPUT_ELEMENT_DESC *vertexDesc, ID3D10Blob* pVertexShaderBuffer, int numElements, ID3D11InputLayout** );

	
	bool createSamplerState(ID3D11Device* pDevice, ID3D11SamplerState** ppSampleState);
	bool createSamplerState(ID3D11Device* pDevice, ID3D11SamplerState** ppSampleState, D3D11_SAMPLER_DESC* samplerDesc);
	bool createConstantBuffer(ID3D11Device*, UINT, ID3D11Buffer**, UINT );

	virtual void RenderShader(ID3D11DeviceContext*, int);
	virtual bool InitShader(ID3D11Device*, WCHAR*, WCHAR*, WCHAR*, D3D11_INPUT_ELEMENT_DESC *vertexDesc, int numElements);

	
protected:
	ID3D11VertexShader* mVertexShader;
	ID3D11PixelShader* mPixelShader;
	ID3D11GeometryShader* mGeometryShader;
	ID3D11HullShader* mHullShader;
	ID3D11DomainShader* mDomainShader;
	ID3D11ComputeShader* mComputeShader;

	ID3D11InputLayout* mLayout;

	ID3D11Buffer* mMatrixBuffer;

};

#endif