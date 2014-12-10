#include "TerrainShaderClass.h"


TerrainShaderClass::TerrainShaderClass() : ShaderClass()
{
	mSampleState = 0;
	mLightBuffer = 0;
	mTextureInfoBuffer = 0;
}


TerrainShaderClass::~TerrainShaderClass()
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
	if (mTextureInfoBuffer)
	{
		mTextureInfoBuffer->Release();
		mTextureInfoBuffer = 0;
	}
}

bool TerrainShaderClass::Init(ID3D11Device* pDevice)
{
	bool result;


	// Initialize the vertex and pixel shaders.
	result = InitShader(pDevice, L"data/shaders/LightVertexShader.hlsl", L"data/shaders/TerrainPixelShader.hlsl", L"data/shaders/LightGeometryShader.hlsl");
	if (!result)
	{
		return false;
	}
	return true;
}


bool TerrainShaderClass::InitShader(ID3D11Device* pDevice, WCHAR* vFileName, WCHAR* pFileName, WCHAR* gFileName)
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

	result = createConstantBuffer(pDevice, sizeof(TerrainCBufferType), &mLightBuffer);
	if (!result)
	{
		return false;
	}

	result = createConstantBuffer(pDevice, sizeof(TextureInfoBufferType), &mTextureInfoBuffer);
	if (!result)
	{
		return false;
	}

	return true;
}


bool TerrainShaderClass::Render(ID3D11DeviceContext* pDeviceContext, ObjectClass* pObject, CameraClass* pCamera, LightObjectClass* pSunLightObject, MaterialClass* pMaterial, FogClass* pDrawDistFog)
{
	bool result;
	unsigned int bufferNumber;



	// Set the shader parameters that it will use for rendering.
	result = ShaderClass::SetConstantBufferParameters(pDeviceContext, pObject->GetWorldMatrix(), pCamera->GetViewMatrix(), pCamera->GetProjMatrix());
	if (!result)
	{
		return false;
	}



	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Set the constant buffer in the vertex shader with the updated values.
	pDeviceContext->GSSetConstantBuffers(bufferNumber, 1, &mMatrixBuffer);

	TextureClass* pTexture = pObject->GetTexture();

	result = SetConstantBufferParameters(pDeviceContext, pSunLightObject, pMaterial, pDrawDistFog);
	if (!result)
	{
		return false;
	}
	result = SetTextureConstantBufferParamters(pDeviceContext, pTexture);
	if (!result)
	{
		return false;
	}
	

	ID3D11ShaderResourceView** tex = pTexture->GetShaderResourceView();

	// Set shader texture resource in the pixel shader.
	pDeviceContext->PSSetShaderResources(0, pTexture->GetTextureCount(), tex);

	if (pTexture->blendEnabled())
	{
		ID3D11ShaderResourceView* tex2 = pTexture->GetBlendMapShaderResourceView();
		pDeviceContext->PSSetShaderResources(3, 1, &tex2);
	}


	// Now render the prepared buffers with the shader.
	RenderShader(pDeviceContext, pObject->GetIndexCount());

	return true;
}


void TerrainShaderClass::RenderShader(ID3D11DeviceContext* pDeviceContext, int indexCount)
{

	pDeviceContext->PSSetSamplers(0, 1, &mSampleState);

	// Set parameters and render mesh
	ShaderClass::RenderShader(pDeviceContext, indexCount);
}

bool TerrainShaderClass::SetConstantBufferParameters(ID3D11DeviceContext* pDeviceContext, LightObjectClass* pSunLightObject, MaterialClass* pMaterial, FogClass* pDrawDistFog)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	TerrainCBufferType* dataPtr;
	unsigned int bufferNumber;

	// Lock the constant buffer so it can be written to.
	result = pDeviceContext->Map(mLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (TerrainCBufferType*)mappedResource.pData;



	// Copy the matrices into the constant buffer.
	dataPtr->ambientColor = pSunLightObject->GetAmbientLight()->GetLightColor();
	dataPtr->ambientReflection = pMaterial->GetAmbientReflection();

	LightClass* temp = pSunLightObject->GetDiffuseLight();

	dataPtr->diffDir = temp->GetLightDir();
	dataPtr->diffColor = temp->GetLightColor();
	dataPtr->diffReflection = pMaterial->GetDiffuseReflection();

	temp = pSunLightObject->GetSpecularLight();

	dataPtr->specDir = temp->GetLightDir();
	dataPtr->specColor = temp->GetLightColor();
	dataPtr->specReflection = pMaterial->GetSpecularReflection();
	dataPtr->specShinyPower = pMaterial->GetSpecularShinyPower();

	dataPtr->fogColor = pDrawDistFog->GetColor();
	dataPtr->fogRange = pDrawDistFog->GetRange();

	// Unlock the constant buffer.
	pDeviceContext->Unmap(mLightBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Set the constant buffer in the shader with the updated values.
	pDeviceContext->PSSetConstantBuffers(bufferNumber, 1, &mLightBuffer);

	return true;
}

bool TerrainShaderClass::SetTextureConstantBufferParamters(ID3D11DeviceContext* pDeviceContext, TextureClass* pTexture)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	TextureInfoBufferType* dataPtr;
	unsigned int bufferNumber;

	// Lock the constant buffer so it can be written to.
	result = pDeviceContext->Map(mTextureInfoBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (TextureInfoBufferType*)mappedResource.pData;


	dataPtr->textureCount = 20;
	dataPtr->useBlendMap = pTexture->blendEnabled();


	// Unlock the constant buffer.
	pDeviceContext->Unmap(mTextureInfoBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 1;

	// Set the constant buffer in the shader with the updated values.
	pDeviceContext->PSSetConstantBuffers(bufferNumber, 1, &mTextureInfoBuffer);

	return true;
}