#include "TerrainShaderClass.h"


TerrainShaderClass::TerrainShaderClass() : ShaderClass()
{
	mSampleState = 0;
	mLightBuffer = 0;
	mDeferredPS = 0;
	mShadowBuffer = 0;
	mShadowDeferredVS = 0;
	mShadowDeferredPS = 0;
	mPointSampleState = 0;
	mShadowDeferredGS = 0;

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
	if (mDeferredPS)
	{
		mDeferredPS->Release();
		mDeferredPS = 0;
	}
	if (mShadowBuffer)
	{
		mShadowBuffer->Release();
		mShadowBuffer = 0;
	}
	if (mShadowDeferredVS)
	{
		mShadowDeferredVS->Release();
		mShadowDeferredVS = 0;
	}
	if (mShadowDeferredPS)
	{
		mShadowDeferredPS->Release();
		mShadowDeferredPS = 0;
	}
	if (mPointSampleState)
	{
		mPointSampleState->Release();
		mPointSampleState = 0;
	}
	if (mShadowDeferredGS)
	{
		mShadowDeferredGS->Release();
		mShadowDeferredGS = 0;
	}
	
}

bool TerrainShaderClass::Init(ID3D11Device* pDevice)
{
	bool result;


	// Initialize the vertex and pixel shaders.
	result = InitShader(pDevice, L"data/shaders/TerrainVertexShader.hlsl", L"data/shaders/TerrainPixelShader.hlsl", L"data/shaders/TerrainGeometryShader.hlsl");
	if (!result)
	{
		return false;
	}
	return true;
}


bool TerrainShaderClass::InitShader(ID3D11Device* pDevice, WCHAR* vFileName, WCHAR* pFileName, WCHAR* gFileName)
{
	bool result;

	D3D11_INPUT_ELEMENT_DESC polygonLayout[6];/* =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		
	};*/

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;


	polygonLayout[3].SemanticName = "TEXCOORD";
	polygonLayout[3].SemanticIndex = 1;
	polygonLayout[3].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[3].InputSlot = 0;
	polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[3].InstanceDataStepRate = 0;

	polygonLayout[4].SemanticName = "TANGENT";
	polygonLayout[4].SemanticIndex = 0;
	polygonLayout[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[4].InputSlot = 0;
	polygonLayout[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[4].InstanceDataStepRate = 0;

	polygonLayout[5].SemanticName = "BINORMAL";
	polygonLayout[5].SemanticIndex = 0;
	polygonLayout[5].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[5].InputSlot = 0;
	polygonLayout[5].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[5].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[5].InstanceDataStepRate = 0;



	// Get a count of the elements in the layout.
	int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	result = ShaderClass::InitShader(pDevice, vFileName, pFileName, gFileName, polygonLayout, numElements);
	if (!result)
	{
		return false;
	}

	result = createSamplerState(pDevice, &mSampleState);
	if (!result)
	{
		return false;
	}

	// Deferred init
	result = createConstantBuffer(pDevice, sizeof(TerrainCBufferType), &mLightBuffer, D3D11_BIND_CONSTANT_BUFFER);
	if (!result)
	{
		return false;
	}

	result = createPixelShader(pDevice, L"data/shaders/DeferredTerrainPixelShader.hlsl", "PSMain", &mDeferredPS);
	if (!result)
	{
		return false;
	}



	// Shadow Init
	result = createConstantBuffer(pDevice, sizeof(ShadowBuffer), &mShadowBuffer, D3D11_BIND_CONSTANT_BUFFER);
	if (!result)
	{
		return false;
	}

	result = createVertexShader(pDevice, L"data/shaders/ShadowDeferredVS.hlsl", "VSMain", &mShadowDeferredVS);
	if (!result)
	{
		return false;
	}

	result = createGeometryShader(pDevice, L"data/shaders/ShadowDeferredGS.hlsl", "GSMain", &mShadowDeferredGS);
	if (!result)
		return false;


	result = createPixelShader(pDevice, L"data/shaders/ShadowDeferredPS.hlsl", "PSMain", &mShadowDeferredPS);
	if (!result)
	{
		return false;
	}

	D3D11_SAMPLER_DESC samplerDesc;
	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;//D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 1000;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = createSamplerState(pDevice, &mPointSampleState, &samplerDesc);
	if (!result)
	{
		return false;
	}
	return true;
}


bool TerrainShaderClass::Render(ID3D11DeviceContext* pDeviceContext, ObjectClass* pObject, CameraClass* pCamera, LightObjectClass* pSunLightObject, FogClass* pDrawDistFog)
{
	bool result;
	unsigned int bufferNumber;



	// Set the shader parameters that it will use for rendering.
	result = ShaderClass::SetConstantBufferParameters(pDeviceContext, pObject->GetWorldMatrix(), pCamera->GetViewMatrix(), pCamera->GetProjMatrix(), pCamera->GetForward());
	if (!result)
	{
		return false;
	}

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Set the constant buffer in the vertex shader with the updated values.
	pDeviceContext->GSSetConstantBuffers(bufferNumber, 1, &mMatrixBuffer);

	TextureClass* pTexture = pObject->GetTexture();

	result = SetConstantBufferParameters(pDeviceContext, pSunLightObject, pDrawDistFog);
	if (!result)
	{
		return false;
	}
	

	ID3D11ShaderResourceView** tex = pTexture->GetShaderResourceView();

	// Set shader texture resource in the pixel shader.
	pDeviceContext->PSSetShaderResources(0, pTexture->GetTextureCount(), tex);


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

bool TerrainShaderClass::SetConstantBufferParameters(ID3D11DeviceContext* pDeviceContext, LightObjectClass* pSunLightObject, FogClass* pDrawDistFog)
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
	
	dataPtr->ambientReflection = XMFLOAT3(0.1, 0.1, 0.1);


	LightClass* temp = pSunLightObject->GetDiffuseLight();

	dataPtr->diffDir = temp->GetLightDir();
	dataPtr->diffColor = temp->GetLightColor();
	dataPtr->diffReflection = XMFLOAT3(1, 1, 1);

	temp = pSunLightObject->GetSpecularLight();

	dataPtr->specDir = temp->GetLightDir();
	dataPtr->specColor = temp->GetLightColor();
	dataPtr->specReflection = XMFLOAT3(1, 1, 1);
	//dataPtr->specShinyPower = pMaterial->GetSpecularShinyPower();

	dataPtr->fogColor = pDrawDistFog->GetColor();
	//dataPtr->fogRange = pDrawDistFog->GetRange();

	XMFLOAT4 values = XMFLOAT4(2, 1.0f, 1, pDrawDistFog->GetRange());
	dataPtr->TCBMSPFR = values;

	// Unlock the constant buffer.
	pDeviceContext->Unmap(mLightBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Set the constant buffer in the shader with the updated values.
	pDeviceContext->PSSetConstantBuffers(bufferNumber, 1, &mLightBuffer);

	return true;
}



bool TerrainShaderClass::RenderDeferred(ID3D11DeviceContext* pDeviceContext, ObjectClass* pObject, CameraClass* pCamera)
{
	bool result;
	unsigned int bufferNumber;



	// Set the shader parameters that it will use for rendering.
	result = ShaderClass::SetConstantBufferParameters(pDeviceContext, pObject->GetWorldMatrix(), pCamera->GetViewMatrix(), pCamera->GetProjMatrix(), pCamera->GetForward());
	if (!result)
	{
		return false;
	}



	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Set the constant buffer in the vertex shader with the updated values.
	pDeviceContext->GSSetConstantBuffers(bufferNumber, 1, &mMatrixBuffer);

	TextureClass* pTexture = pObject->GetTexture();

	ID3D11ShaderResourceView** tex = pTexture->GetShaderResourceView();

	// Set shader texture resource in the pixel shader.
	pDeviceContext->PSSetShaderResources(0, pTexture->GetTextureCount(), tex);

	if (pTexture->blendEnabled())
	{
		ID3D11ShaderResourceView* tex2 = pTexture->GetBlendMapShaderResourceView();
		pDeviceContext->PSSetShaderResources(3, 1, &tex2);
	}


	// Now render the prepared buffers with the shader.
	pDeviceContext->PSSetSamplers(0, 1, &mSampleState);

	// Set the vertex input layout.
	pDeviceContext->IASetInputLayout(mLayout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	pDeviceContext->VSSetShader(mVertexShader, nullptr, 0);
	pDeviceContext->HSSetShader(mHullShader, nullptr, 0);
	pDeviceContext->DSSetShader(mDomainShader, nullptr, 0);
	pDeviceContext->GSSetShader(mGeometryShader, nullptr, 0);
	pDeviceContext->PSSetShader(mDeferredPS, nullptr, 0);

	// Render mesh stored in active buffers
	pDeviceContext->DrawIndexed(pObject->GetIndexCount(), 0, 0);

	return true;
}

bool TerrainShaderClass::RenderShadowsDeferred(ID3D11DeviceContext* pDeviceContext, ObjectClass* pObject, CameraClass* pCamera, PointLightClass* pLights, ID3D11ShaderResourceView* pShadowmap)
{
	bool result;
	unsigned int bufferNumber;



	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	result = SetShadowConstantBufferParamters(pDeviceContext, pObject, pCamera, pLights);
	if (!result)
	{
		return false;
	}
	ModelClass* pModel = pObject->GetModel();
	
	pDeviceContext->PSSetShaderResources(0, 1, &pShadowmap);



	TextureClass* pTexture = pModel->GetDetailMap();

	ID3D11ShaderResourceView** tex = pTexture->GetShaderResourceView();

	// Set shader texture resource in the pixel shader.
	pDeviceContext->PSSetShaderResources(1, 1, tex);


	pTexture = pModel->GetNormalMap();

	tex = pTexture->GetShaderResourceView();

	// Set shader texture resource in the pixel shader.
	pDeviceContext->PSSetShaderResources(2, 1, tex);


	pTexture = pModel->GetTexture();

	tex = pTexture->GetShaderResourceView();

	// Set shader texture resource in the pixel shader.
	pDeviceContext->PSSetShaderResources(3, pTexture->GetTextureCount(), tex);

	

	// Set shader texture resource in the pixel shader.
	pDeviceContext->PSSetShaderResources(1, pTexture->GetTextureCount(), tex);

	pDeviceContext->PSSetSamplers(1, 1, &mPointSampleState);
	// Now render the prepared buffers with the shader.
	pDeviceContext->PSSetSamplers(0, 1, &mSampleState);

	// Set the vertex input layout.
	pDeviceContext->IASetInputLayout(mLayout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	pDeviceContext->VSSetShader(mShadowDeferredVS, nullptr, 0);
	pDeviceContext->HSSetShader(mHullShader, nullptr, 0);
	pDeviceContext->DSSetShader(mDomainShader, nullptr, 0);
	pDeviceContext->GSSetShader(mShadowDeferredGS, nullptr, 0);
	pDeviceContext->PSSetShader(mShadowDeferredPS, nullptr, 0);

	// Render mesh stored in active buffers
	pDeviceContext->DrawIndexed(pObject->GetIndexCount(), 0, 0);

	return true;
}

bool TerrainShaderClass::RenderShadowsDeferred(ID3D11DeviceContext* pDeviceContext, ObjectClass* pObject, CameraClass* pCamera, PointLightClass* pLights, ID3D11ShaderResourceView* pShadowmap, UINT indexCount, UINT indexStart)
{
	bool result;
	unsigned int bufferNumber;



	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	result = SetShadowConstantBufferParamters(pDeviceContext, pObject, pCamera, pLights);
	if (!result)
	{
		return false;
	}

	ModelClass* pModel = pObject->GetModel();

	pDeviceContext->PSSetShaderResources(0, 1, &pShadowmap);



	TextureClass* pTexture = pModel->GetDetailMap();

	ID3D11ShaderResourceView** tex = pTexture->GetShaderResourceView();

	// Set shader texture resource in the pixel shader.
	pDeviceContext->PSSetShaderResources(1, 1, tex);

	pTexture = pModel->GetNormalMap();

	tex = pTexture->GetShaderResourceView();

	// Set shader texture resource in the pixel shader.
	pDeviceContext->PSSetShaderResources(2, 1, tex);


	pTexture = pModel->GetTexture();

	tex = pTexture->GetShaderResourceView();

	// Set shader texture resource in the pixel shader.
	pDeviceContext->PSSetShaderResources(3, pTexture->GetTextureCount(), tex);



	pDeviceContext->PSSetSamplers(1, 1, &mPointSampleState);
	// Now render the prepared buffers with the shader.
	pDeviceContext->PSSetSamplers(0, 1, &mSampleState);

	// Set the vertex input layout.
	pDeviceContext->IASetInputLayout(mLayout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	pDeviceContext->VSSetShader(mShadowDeferredVS, nullptr, 0);
	pDeviceContext->HSSetShader(mHullShader, nullptr, 0);
	pDeviceContext->DSSetShader(mDomainShader, nullptr, 0);
	pDeviceContext->GSSetShader(mShadowDeferredGS, nullptr, 0);
	pDeviceContext->PSSetShader(mShadowDeferredPS, nullptr, 0);

	// Render mesh stored in active buffers
	pDeviceContext->DrawIndexed(indexCount, indexStart, 0);

	return true;
}

bool TerrainShaderClass::SetShadowConstantBufferParamters(ID3D11DeviceContext* pDeviceContext, ObjectClass* pObject, CameraClass* pCamera, PointLightClass* pPointLight)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ShadowBuffer* dataPtr;
	unsigned int bufferNumber;

	// Lock the constant buffer so it can be written to.
	result = pDeviceContext->Map(mShadowBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (ShadowBuffer*)mappedResource.pData;
	
	XMMATRIX world = XMLoadFloat4x4(&pObject->GetWorldMatrix());

	XMMATRIX view = XMLoadFloat4x4(&pCamera->GetViewMatrix());
	XMMATRIX proj = XMLoadFloat4x4(&pCamera->GetProjMatrix());

	XMMATRIX lview = XMLoadFloat4x4(&pPointLight->GetViewMatrix());
	XMMATRIX lproj = XMLoadFloat4x4(&pPointLight->GetProjMatrix());

	XMMATRIX lWVP = XMMatrixTranspose(world*lview*lproj);
	XMMATRIX WVP = XMMatrixTranspose(world*view*proj);


	XMStoreFloat4x4(&dataPtr->worldViewProj, WVP);
	XMStoreFloat4x4(&dataPtr->world, world);
	XMStoreFloat4x4(&dataPtr->LightWorldViewProj, lWVP);
	XMFLOAT3 cP = pCamera->GetPosition();
	dataPtr->camPos = XMFLOAT4(cP.x, cP.y, cP.z, 0);

	// Unlock the constant buffer.
	pDeviceContext->Unmap(mShadowBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Set the constant buffer in the shader with the updated values.
	pDeviceContext->GSSetConstantBuffers(bufferNumber, 1, &mShadowBuffer);

	return true;
}