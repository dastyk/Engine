#ifndef _SHADERCLASS_H_
#define _SHADERCLASS_H_

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

struct MatrixBufferType
{
	XMFLOAT4X4 mWorldViewProj;
	XMFLOAT4X4 mWorld;
	XMFLOAT4X4 mWorldView;
};

class ShaderClass
{
public:
	ShaderClass();
	virtual ~ShaderClass();

	virtual bool Init(ID3D11Device* pDevice) = 0;
	virtual bool Render(ID3D11DeviceContext* pDeviceContext, int vertexCount, CameraClass* pCamera);
	virtual bool Render(ID3D11DeviceContext* pDeviceContext, int indexCount, XMFLOAT4X4& worldMatrix, XMFLOAT4X4& viewMatrix, XMFLOAT4X4& projMatrix);
	virtual bool Render(ID3D11DeviceContext* pDeviceContext, int indexCount, XMFLOAT4X4& worldMatrix, XMFLOAT4X4& viewMatrix, XMFLOAT4X4& projMatrix, ID3D11ShaderResourceView* pTexture);
	virtual bool Render(ID3D11DeviceContext* pDeviceContext, ObjectClass* pObject, CameraClass* pCamera, LightObjectClass* pSunLightObject, MaterialClass* pMaterial, FogClass* pDrawDistFog);

protected:
	void OutputShaderErrorMessage(ID3D10Blob*, WCHAR*);

	bool SetConstantBufferParameters(ID3D11DeviceContext* pDeviceContext, XMFLOAT4X4& worldMatrix, XMFLOAT4X4& viewMatrix, XMFLOAT4X4& projMatrix);

	bool createVertexShaderAndInputLayout(ID3D11Device* pDevice, WCHAR* fileName, CHAR* EntryPoint, D3D11_INPUT_ELEMENT_DESC *vertexDesc, int numElements);
	bool createPixelShader(ID3D11Device* pDevice, WCHAR* fileName, CHAR* EntryPoint);
	bool createGeometryShader(ID3D11Device* pDevice, WCHAR* fileName, CHAR* EntryPoint);
	bool createInputLayout(ID3D11Device* pDevice, D3D11_INPUT_ELEMENT_DESC *vertexDesc, ID3D10Blob* pVertexShaderBuffer, int numElements);
	bool createSamplerState(ID3D11Device* pDevice, ID3D11SamplerState** ppSampleState);
	bool createConstantBuffer(ID3D11Device*, UINT, ID3D11Buffer**);

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