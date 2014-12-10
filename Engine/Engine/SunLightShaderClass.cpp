#include "SunLightShaderClass.h"


SunLightShaderClass::SunLightShaderClass() : ShaderClass()
{
	mSampleState = 0;
	mLightBuffer = 0;
}


SunLightShaderClass::~SunLightShaderClass()
{
	if (mSampleState)
	{
		mSampleState->Release();
		mSampleState = 0;
	}
	if (mLightBuffer)
	{
		mLightBuffer->Release();
		mLightBuffer = 0;
	}
}

bool SunLightShaderClass::Init(ID3D11Device* pDevice)
{
	bool result;


	// Initialize the vertex and pixel shaders.
	result = InitShader(pDevice, L"data/shaders/LightVertexShader.hlsl", L"data/shaders/LightPixelShader.hlsl", L"data/shaders/LightGeometryShader.hlsl");
	if (!result)
	{
		return false;
	}
	return true;
}


bool SunLightShaderClass::InitShader(ID3D11Device* pDevice, WCHAR* vFileName, WCHAR* pFileName, WCHAR* gFileName)
{
	bool result;

	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// Get a count of the elements in the layout.
	int numElements = sizeof(vertexDesc) / sizeof(vertexDesc[0]);

	result = ShaderClass::InitShader(pDevice, vFileName, pFileName, gFileName, vertexDesc, numElements);
	if (!result)
	{
		return false;
	}

	result = createSamplerState(pDevice, &mSampleState);
	if (!result)
	{
		return false;
	}

	result = createConstantBuffer(pDevice, sizeof(LightCBufferType), &mLightBuffer);
	if (!result)
	{
		return false;
	}


	return true;
}


bool SunLightShaderClass::Render(ID3D11DeviceContext* pDeviceContext, int indexCount, XMFLOAT4X4& worldMatrix, XMFLOAT4X4& viewMatrix, XMFLOAT4X4& projMatrix, ID3D11ShaderResourceView* pTexture, XMFLOAT3& ambientColor, XMFLOAT3& diffDir, XMFLOAT3& diffColor, XMFLOAT3& specPos, XMFLOAT3& specColor, float specShinyPower, XMFLOAT3& fogColor, float fogRange)
{
	bool result;
	unsigned int bufferNumber;



	// Set the shader parameters that it will use for rendering.
	result = ShaderClass::SetConstantBufferParameters(pDeviceContext, worldMatrix, viewMatrix, projMatrix);
	if (!result)
	{
		return false;
	}
	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Set the constant buffer in the vertex shader with the updated values.
	pDeviceContext->GSSetConstantBuffers(bufferNumber, 1, &mMatrixBuffer);

	result = SetConstantBufferParameters(pDeviceContext, ambientColor, diffDir, diffColor, specPos, specColor, specShinyPower, fogColor, fogRange);
	if (!result)
	{
		return false;
	}

	// Set shader texture resource in the pixel shader.
	pDeviceContext->PSSetShaderResources(0, 1, &pTexture);

	// Now render the prepared buffers with the shader.
	RenderShader(pDeviceContext, indexCount);

	return true;
}


void SunLightShaderClass::RenderShader(ID3D11DeviceContext* pDeviceContext, int indexCount)
{

	pDeviceContext->PSSetSamplers(0, 1, &mSampleState);

	// Set parameters and render mesh
	ShaderClass::RenderShader(pDeviceContext, indexCount);
}

bool SunLightShaderClass::SetConstantBufferParameters(ID3D11DeviceContext* pDeviceContext, XMFLOAT3& ambientColor, XMFLOAT3& diffDir, XMFLOAT3& diffColor, XMFLOAT3& specDir, XMFLOAT3& specColor, float specShinyPower, XMFLOAT3& fogColor, float fogRange)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	LightCBufferType* dataPtr;
	unsigned int bufferNumber;

	// Lock the constant buffer so it can be written to.
	result = pDeviceContext->Map(mLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (LightCBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->AmbientColor = ambientColor;
	dataPtr->diffDir = diffDir;
	dataPtr->diffColor = diffColor;
	dataPtr->specDir = specDir;
	dataPtr->specColor = specColor;
	dataPtr->specShinyPower = specShinyPower;
	dataPtr->fogColor = fogColor;
	dataPtr->fogRange = fogRange;

	// Unlock the constant buffer.
	pDeviceContext->Unmap(mLightBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Set the constant buffer in the shader with the updated values.
	pDeviceContext->PSSetConstantBuffers(bufferNumber, 1, &mLightBuffer);

	return true;
}