#include "DeferredShaderClass.h"


DeferredShaderClass::DeferredShaderClass() : ShaderClass()
{
	mSampleState = 0;
	mLightBuffer = 0;
}


DeferredShaderClass::~DeferredShaderClass()
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

bool DeferredShaderClass::Init(ID3D11Device* pDevice)
{
	bool result;


	// Initialize the vertex and pixel shaders.
	result = InitShader(pDevice, L"data/shaders/DeferredVertexShader.hlsl", L"data/shaders/DeferredPixelShader.hlsl", NULL,
		L"data/shaders/CompleDeferredVertexShader.hlsl", L"data/shaders/CompleDeferredPixelShader.hlsl", L"data/shaders/CompleDeferredGeometryShader.hlsl");
	if (!result)
	{
		return false;
	}

	return true;
}

bool DeferredShaderClass::RenderDeferred(ID3D11DeviceContext* pDeviceContext, ObjectClass* pObject, CameraClass* pCamera)
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
	pDeviceContext->VSSetConstantBuffers(bufferNumber, 1, &mMatrixBuffer);

	TextureClass* pTexture = pObject->GetTexture();
	ID3D11ShaderResourceView** tex = pTexture->GetShaderResourceView();

	// Set shader texture resource in the pixel shader.
	pDeviceContext->PSSetShaderResources(0, pTexture->GetTextureCount(), tex);

	// Now render the prepared buffers with the shader.
	RenderShader(pDeviceContext, pObject->GetIndexCount());

	return true;
}

bool DeferredShaderClass::Render(ID3D11DeviceContext* pDeviceContext, DeferredBufferClass* pBuffer)
{
	bool result;
	unsigned int bufferNumber;

	unsigned int stride = sizeof(DeferredVertexStruct);
	unsigned int offset = 0;

	pDeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;


	//result = SetConstantBufferParameters(pDeviceContext, pSunLightObject, pMaterial, pDrawDistFog);


	ID3D11ShaderResourceView** tex = pBuffer->GetShaderResourceView();

	// Set shader texture resource in the pixel shader.
	pDeviceContext->PSSetShaderResources(0, BUFFER_COUNT, tex);

	

	pDeviceContext->PSSetSamplers(0, 1, &mSampleStateRenderTarget);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	pDeviceContext->VSSetShader(mVertexShaderRenderTarget, nullptr, 0);
	pDeviceContext->HSSetShader(nullptr, nullptr, 0);
	pDeviceContext->DSSetShader(nullptr, nullptr, 0);
	pDeviceContext->GSSetShader(nullptr, nullptr, 0);
	pDeviceContext->PSSetShader(mPixelShaderRenderTarget, nullptr, 0);

	// Render mesh stored in active buffers
	pDeviceContext->Draw(6, 0);


	return true;
}



bool DeferredShaderClass::InitShader(ID3D11Device* pDevice, WCHAR* dVertex, WCHAR* dPixel, WCHAR* dGeometry, WCHAR* vertex, WCHAR* pixel, WCHAR* geometry)
{

	bool result;
	HRESULT hr;
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// Get a count of the elements in the layout.
	int numElements = sizeof(vertexDesc) / sizeof(vertexDesc[0]);

	result = ShaderClass::InitShader(pDevice, dVertex, dPixel, dGeometry, vertexDesc, numElements);
	if (!result)
	{
		return false;
	}



	D3D11_INPUT_ELEMENT_DESC vertexDesc2[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// Get a count of the elements in the layout.
	numElements = sizeof(vertexDesc2) / sizeof(vertexDesc2[0]);

	result = createVertexShaderAndInputLayout(pDevice, vertex, "VSMain", vertexDesc2, numElements, &mVertexShaderRenderTarget, &mLayout);
	if (!result)
	{
		return false;
	}


	result = createPixelShader(pDevice, pixel, "PSMain", &mPixelShaderRenderTarget);
	if (!result)
	{
		return false;
	}

	result = createSamplerState(pDevice, &mSampleState);
	if (!result)
	{
		return false;
	}


	D3D11_SAMPLER_DESC samplerDesc;
	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;//D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = createSamplerState(pDevice, &mSampleStateRenderTarget, &samplerDesc);
	if (!result)
	{
		return false;
	}

	DeferredVertexStruct vertices[] =
	{
		{ XMFLOAT3(-1.0f, 1.0f, 0.0f), XMFLOAT2(0.0,0.0) },
		{ XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0, 0.0) },
		{ XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT2(0.0, 1.0) },

		{ XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT2(0.0, 1.0) },
		{ XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0, 0.0) },
		{ XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT2(1.0, 1.0) }


	};


	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	vinitData.SysMemPitch = 0;
	vinitData.SysMemSlicePitch = 0;

	D3D11_BUFFER_DESC vbd;
	memset(&vbd, 0, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(DeferredVertexStruct)* 6;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	hr = pDevice->CreateBuffer(&vbd, &vinitData, &mVertexBuffer);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to create Vertex Buffer.", 0, 0);
		return false;
	}


	result = createConstantBuffer(pDevice, sizeof(DeferredLightConstantBuffer), &mLightBuffer);
	if (!result)
	{
		return false;
	}



	return true;

}

void DeferredShaderClass::RenderShader(ID3D11DeviceContext* pDeviceContext, int indexCount)
{
	pDeviceContext->PSSetSamplers(0, 1, &mSampleState);

	ShaderClass::RenderShader(pDeviceContext, indexCount);
	return;
}


//bool DeferredShaderClass::SetConstantBufferParameters(ID3D11DeviceContext* pDeviceContext, CameraClass* pCamera, PointLightClass** ppLights, int LightCount, MaterialClass* pMaterial, FogClass* pDrawDistFog)
//{
//	HRESULT result;
//	D3D11_MAPPED_SUBRESOURCE mappedResource;
//	DeferredLightConstantBuffer* dataPtr;
//	unsigned int bufferNumber;
//
//	// Lock the constant buffer so it can be written to.
//	result = pDeviceContext->Map(mLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
//	if (FAILED(result))
//	{
//		return false;
//	}
//
//	// Get a pointer to the data in the constant buffer.
//	dataPtr = (DeferredLightConstantBuffer*)mappedResource.pData;
//
//	XMFLOAT4 values = XMFLOAT4(LightCount, pDrawDistFog->GetRange(), pMaterial->GetSpecularShinyPower(), 0);
//	dataPtr->LightCount_FogRange_ShinyPower_Unused = values;
//
//	dataPtr->ambientReflection = pMaterial->GetAmbientReflection();
//
//	dataPtr->diffReflection = pMaterial->GetDiffuseReflection();
//
//	dataPtr->specReflection = pMaterial->GetSpecularReflection();
//
//	dataPtr->fogColor = pDrawDistFog->GetColor();
//
//	dataPtr->CamPos = pCamera->GetPosition();
//
//	for (int i = 0; i < values.x; i++)
//	{
//		dataPtr->lights[i].ambientColor = ppLights[i]->GetAmbientColor;
//	}
//
//	// Unlock the constant buffer.
//	pDeviceContext->Unmap(mLightBuffer, 0);
//
//	// Set the position of the constant buffer in the vertex shader.
//	bufferNumber = 0;
//
//	// Set the constant buffer in the shader with the updated values.
//	pDeviceContext->PSSetConstantBuffers(bufferNumber, 1, &mLightBuffer);
//
//	return true;
//}