#include "DeferredShaderClass.h"


DeferredShaderClass::DeferredShaderClass() : ShaderClass()
{
	mSampleState = 0;
	mSampleStateRenderTarget = 0;
	mLightBuffer = 0;
	for (int i = 0; i < BUFFER_COUNT; i++)
	{
		unbindSrv[i] = nullptr;
	}
	mVertexShaderRenderTarget = 0;
	mPixelShaderRenderTarget = 0;
	mVertexBuffer = 0;
	mLayout2 = 0;
	mMaterialBuffer = 0;
	mLightPS = 0;
	mCamBuffer = 0;
	mBlendState = 0;
	mDepthStencilState = 0;
	mLightGS = 0;
	mVPBuffer = 0;
	mLightVS = 0;
}


DeferredShaderClass::~DeferredShaderClass()
{
	if (mSampleState)
	{
		mSampleState->Release();
		mSampleState = 0;
	}
	if (mSampleStateRenderTarget)
	{
		mSampleStateRenderTarget->Release();
		mSampleStateRenderTarget = 0;
	}
	if (mLightBuffer)
	{
		mLightBuffer->Release();
		mLightBuffer = 0;
	}
	if (mVertexShaderRenderTarget)
	{
		mVertexShaderRenderTarget->Release();
		mVertexShaderRenderTarget = 0;
	}
	if (mPixelShaderRenderTarget)
	{
		mPixelShaderRenderTarget->Release();
		mPixelShaderRenderTarget = 0;
	}
	if (mVertexBuffer)
	{
		mVertexBuffer->Release();
		mVertexBuffer = 0;
	}
	if (mLayout2)
	{
		mLayout2->Release();
		mLayout2 = 0;
	}
	if (mMaterialBuffer)
	{
		mMaterialBuffer->Release();
		mMaterialBuffer = 0;
	}
	if (mLightPS)
	{
		mLightPS->Release();
		mLightPS = 0;
	}
	if (mCamBuffer)
	{
		mCamBuffer->Release();
		mCamBuffer = 0;
	}
	if (mBlendState)
	{
		mBlendState->Release();
		mBlendState = 0;
	}
	if (mDepthStencilState)
	{
		mDepthStencilState->Release();
		mDepthStencilState = 0;
	}

	if (mLightGS)
	{
		mLightGS->Release();
		mLightGS = 0;
	}
	if (mVPBuffer)
	{
		mVPBuffer->Release();
		mVPBuffer = 0;
	}
	if (mLightVS)
	{
		mLightVS->Release();
		mLightVS = 0;
	}
	
}

bool DeferredShaderClass::Init(ID3D11Device* pDevice)
{
	bool result;


	// Initialize the vertex and pixel shaders.
	result = InitShader(pDevice, L"data/shaders/DeferredVertexShader.hlsl", L"data/shaders/DeferredPixelShader.hlsl", L"data/shaders/DeferredGeometryShader.hlsl",
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
	result = ShaderClass::SetConstantBufferParameters(pDeviceContext, pObject->GetWorldMatrix(), pCamera->GetViewMatrix(), pCamera->GetProjMatrix(), pCamera->GetForward());
	if (!result)
	{
		return false;
	}

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Set the constant buffer in the vertex shader with the updated values.
	pDeviceContext->GSSetConstantBuffers(bufferNumber, 1, &mMatrixBuffer);
	
	ModelClass* pModel = pObject->GetModel();
	UINT count = pModel->GetObjectCount();
	MatrialDesc* pMaterialDesc = pModel->GetMaterials();
	SubsetTableDesc* pSubSet = pModel->GetSubSetTable();



	// Now render the prepared buffers with the shader.
	pDeviceContext->PSSetSamplers(0, 1, &mSampleState);

	// Set the vertex input layout.
	pDeviceContext->IASetInputLayout(mLayout);


	// Set the vertex and pixel shaders that will be used to render this triangle.
	pDeviceContext->VSSetShader(mVertexShader, nullptr, 0);
	pDeviceContext->HSSetShader(mHullShader, nullptr, 0);
	pDeviceContext->DSSetShader(mDomainShader, nullptr, 0);
	pDeviceContext->GSSetShader(mGeometryShader, nullptr, 0);
	pDeviceContext->PSSetShader(mPixelShader, nullptr, 0);

	struct MaterialBuffer{
		MatrialDesc* pMaterials;
		ObjectClass* pObjects;
	};


	for (UINT i = 0; i < count; i++)
	{
		TextureClass* pTexture = pObject->GetTexture();
		ID3D11ShaderResourceView** tex = pTexture->GetShaderResourceView();
	
		result = SetMaterialConstantBufferParameters(pDeviceContext, &pMaterialDesc[pSubSet[i].SubsetID]);
		if (!result)
		{
			return false;
		}
		// Set shader texture resource in the pixel shader.
		pDeviceContext->PSSetShaderResources(0, 1, &tex[pSubSet[i].SubsetID]);

		// Render mesh stored in active buffers
		pDeviceContext->DrawIndexed(pSubSet[i].FaceCount * 3, pSubSet[i].FaceStart * 3, 0);
	
	}

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

	ID3D11ShaderResourceView* tex = pBuffer->GetLightSRV();

	//// Set shader texture resource in the pixel shader.
	pDeviceContext->PSSetShaderResources(0, 1, &tex);

	tex = pBuffer->GetShaderResourceView(1);

	pDeviceContext->PSSetShaderResources(1, 1, &tex);

	pDeviceContext->PSSetSamplers(0, 1, &mSampleStateRenderTarget);





	// Set the vertex input layout.
	pDeviceContext->IASetInputLayout(mLayout2);


	// Set the vertex and pixel shaders that will be used to render this triangle.
	pDeviceContext->VSSetShader(mVertexShaderRenderTarget, nullptr, 0);
	pDeviceContext->HSSetShader(nullptr, nullptr, 0);
	pDeviceContext->DSSetShader(nullptr, nullptr, 0);
	pDeviceContext->GSSetShader(nullptr, nullptr, 0);
	pDeviceContext->PSSetShader(mPixelShaderRenderTarget, nullptr, 0);

	// Render mesh stored in active buffers
	pDeviceContext->Draw(6, 0);

	pDeviceContext->PSSetShaderResources(0, BUFFER_COUNT, unbindSrv);

	return true;
}

bool DeferredShaderClass::RenderLights(ID3D11DeviceContext* pDeviceContext, CameraClass* pCamera, DeferredBufferClass* pBuffer, PointLightClass** ppLights, UINT NrOfLights)
{
	bool result;
	unsigned int bufferNumber;

	unsigned int stride = sizeof(DeferredVertexStruct);
	unsigned int offset = 0;

	pDeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	ID3D11ShaderResourceView** tex = pBuffer->GetShaderResourceView();

	// Set shader texture resource in the pixel shader.
	pDeviceContext->PSSetShaderResources(0, BUFFER_COUNT, tex);

	pDeviceContext->PSSetSamplers(0, 1, &mSampleStateRenderTarget);


	// Set the vertex input layout.
	pDeviceContext->IASetInputLayout(mLayout2);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	pDeviceContext->VSSetShader(mVertexShaderRenderTarget, nullptr, 0);
	pDeviceContext->HSSetShader(nullptr, nullptr, 0);
	pDeviceContext->DSSetShader(nullptr, nullptr, 0);
	pDeviceContext->GSSetShader(nullptr, nullptr, 0);
	pDeviceContext->PSSetShader(mLightPS, nullptr, 0);

	//pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	result = SetCamConstantBufferParameters(pDeviceContext, pCamera);
	if (!result)
	{
		return false;
	}

	/*result = SetVPConstantBufferParameters(pDeviceContext, pCamera);
	if (!result)
	{
		return false;
	}*/

	float color[4] = { 0.0f };
	float pcolor[4] = { 0.0f };
	ID3D11BlendState* pBS;
	UINT sampleMask = 0xffffffff;
	UINT ref = 1;
	pDeviceContext->OMGetBlendState(&pBS, pcolor, &sampleMask);
	pDeviceContext->OMSetBlendState(mBlendState, color, 0xffffffff);

	pDeviceContext->OMGetDepthStencilState(&prevDSS, &ref);

	// Set the depth stencil state.
	pDeviceContext->OMSetDepthStencilState(mDepthStencilState, 1);



	BoundingFrustum f = pCamera->GetBoundingFrustum();
	BoundingSphere s;
	for (UINT i = 0; i < NrOfLights; i++)
	{
		s = ppLights[i]->GetBoundingSphere();
		if (f.Intersects(s))
		{
			result = SetLightConstantBufferParameters(pDeviceContext, ppLights[i]);
			if (!result)
			{
				return false;
			}

			pDeviceContext->Draw(6, 0);
		}
	}
	
	pDeviceContext->OMSetDepthStencilState(prevDSS, ref);

	pDeviceContext->OMSetBlendState(pBS, pcolor, sampleMask);

	pDeviceContext->PSSetShaderResources(0, BUFFER_COUNT, unbindSrv);

	return true;
}


bool DeferredShaderClass::RenderLights(ID3D11DeviceContext* pDeviceContext, PointLightClass* pLight)
{
	bool result;
	result = SetLightConstantBufferParameters(pDeviceContext, pLight);
	if (!result)
	{
		return false;
	}


	// Render mesh stored in active buffers
	pDeviceContext->Draw(6, 0);



	return true;
}

bool DeferredShaderClass::SetLP(ID3D11DeviceContext* pDeviceContext, CameraClass* pCamera, DeferredBufferClass* pBuffer)
{
	bool result;
	unsigned int bufferNumber;

	unsigned int stride = sizeof(DeferredVertexStruct);
	unsigned int offset = 0;

	pDeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	ID3D11ShaderResourceView** tex = pBuffer->GetShaderResourceView();

	// Set shader texture resource in the pixel shader.
	pDeviceContext->PSSetShaderResources(0, BUFFER_COUNT, tex);

	pDeviceContext->PSSetSamplers(0, 1, &mSampleStateRenderTarget);


	// Set the vertex input layout.
	pDeviceContext->IASetInputLayout(mLayout2);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	pDeviceContext->VSSetShader(mVertexShaderRenderTarget, nullptr, 0);
	pDeviceContext->HSSetShader(nullptr, nullptr, 0);
	pDeviceContext->DSSetShader(nullptr, nullptr, 0);
	pDeviceContext->GSSetShader(nullptr, nullptr, 0);
	pDeviceContext->PSSetShader(mLightPS, nullptr, 0);


	result = SetCamConstantBufferParameters(pDeviceContext, pCamera);
	if (!result)
	{
		return false;
	}

	float color[4] = { 0.0f };


	pDeviceContext->OMGetBlendState(&pBS, pcolor, &sampleMask);
	pDeviceContext->OMSetBlendState(mBlendState, color, 0xffffffff);

	return true;

}
bool DeferredShaderClass::UnSetLP(ID3D11DeviceContext* pDeviceContext)
{
	pDeviceContext->OMSetBlendState(pBS, pcolor, sampleMask);

	pDeviceContext->PSSetShaderResources(0, BUFFER_COUNT, unbindSrv);

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
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
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

	result = createVertexShaderAndInputLayout(pDevice, vertex, "VSMain", vertexDesc2, numElements, &mVertexShaderRenderTarget, &mLayout2);
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


	result = createConstantBuffer(pDevice,sizeof(PointLight), &mLightBuffer, D3D11_BIND_CONSTANT_BUFFER);
	if (!result)
	{
		return false;
	}

	result = createConstantBuffer(pDevice, sizeof(MatrialDescPadded), &mMaterialBuffer, D3D11_BIND_CONSTANT_BUFFER);
	if (!result)
	{
		return false;
	}
	
	result = createConstantBuffer(pDevice, sizeof(CamBuffer), &mCamBuffer, D3D11_BIND_CONSTANT_BUFFER);
	if (!result)
	{
		return false;
	}

	result = createPixelShader(pDevice, L"data/shaders/LightPS.hlsl", "PSMain", &mLightPS);
	if (!result)
	{
		return false;
	}

	result = createGeometryShader(pDevice, L"data/shaders/LightGS.hlsl", "GSMain", &mLightGS);
	if (!result)
	{
		return false;
	}

	result = createVertexShader(pDevice, L"data/shaders/LightVS.hlsl", "VSMain", &mLightVS);
	if (!result)
	{
		return false;
	}


	result = createConstantBuffer(pDevice, sizeof(VPBuffer), &mVPBuffer, D3D11_BIND_CONSTANT_BUFFER);
	if (!result)
	{
		return false;
	}

	D3D11_BLEND_DESC blendStateDescription;
	// Clear the blend state description.
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	// Create an alpha enabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = true;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create the blend state using the description.
	hr = pDevice->CreateBlendState(&blendStateDescription, &mBlendState);
	if (FAILED(hr))
	{
		return false;
	}

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;


	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = false;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	hr = pDevice->CreateDepthStencilState(&depthStencilDesc, &mDepthStencilState);
	if (FAILED(hr))
	{
		MessageBox(0, L"Could not create mDepthStencilState.", 0, 0);
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


//bool DeferredShaderClass::SetConstantBufferParameters(ID3D11DeviceContext* pDeviceContext, PointLightClass** ppLights, UINT NrOfLights, ObjectClass* pObject, FogClass* pDrawDistFog, CameraClass* pCamera)
//{
//	HRESULT result;
//	D3D11_MAPPED_SUBRESOURCE mappedResource;
//	LightConstantBuffer* dataPtr;
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
//	dataPtr = (LightConstantBuffer*)mappedResource.pData;
//
//	ModelClass* pModel = pObject->GetModel();
//	MatrialDesc *pMaterials = pModel->GetMaterials();
//	int oCount = pModel->GetObjectCount();
//
//	dataPtr->LightCount_FogRange_ObjectCount_Unused = XMFLOAT4((float)NrOfLights, (float)pDrawDistFog->GetRange(), (float)oCount, 0.0f);
//	dataPtr->fogColor = pDrawDistFog->GetColor();
//	dataPtr->CamPos = pCamera->GetPosition();
//
//	for (UINT i = 0; i < NrOfLights; i++)
//	{
//		dataPtr->lights[i].Pos = ppLights[i]->GetLightPos();
//		XMFLOAT3 color = ppLights[i]->GetLightColor();
//		dataPtr->lights[i].Color_LightRange = XMFLOAT4(color.x, color.y, color.z, ppLights[i]->GetRadius());
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

bool DeferredShaderClass::SetMaterialConstantBufferParameters(ID3D11DeviceContext* pDeviceContext, MatrialDesc* pMaterial)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrialDescPadded* dataPtr;
	unsigned int bufferNumber;

	// Lock the constant buffer so it can be written to.
	result = pDeviceContext->Map(mMaterialBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrialDescPadded*)mappedResource.pData;

	dataPtr->Ambient = pMaterial->Ambient;
	dataPtr->Diffuse = pMaterial->Diffuse;
	dataPtr->Specular = pMaterial->Specular;
	dataPtr->Reflectivity = pMaterial->Reflectivity;

	dataPtr->SpecPower_AlphaClip_Unused_Unused = XMFLOAT4(pMaterial->SpecPower, pMaterial->AlphaClip, 0, 0);

	// Unlock the constant buffer.
	pDeviceContext->Unmap(mMaterialBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Set the constant buffer in the shader with the updated values.
	pDeviceContext->PSSetConstantBuffers(bufferNumber, 1, &mMaterialBuffer);

	return true;
}

bool DeferredShaderClass::SetLightConstantBufferParameters(ID3D11DeviceContext* pDeviceContext, PointLightClass* pPointLight)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	PointLight* dataPtr;
	unsigned int bufferNumber;

	// Lock the constant buffer so it can be written to.
	result = pDeviceContext->Map(mLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (PointLight*)mappedResource.pData;

	dataPtr->Pos = pPointLight->GetLightPos();
	XMFLOAT3 color = pPointLight->GetLightColor();
	dataPtr->Color_LightRange = XMFLOAT4(color.x, color.y, color.z, pPointLight->GetRadius());

	// Unlock the constant buffer.
	pDeviceContext->Unmap(mLightBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Set the constant buffer in the shader with the updated values.
	pDeviceContext->PSSetConstantBuffers(bufferNumber, 1, &mLightBuffer);

	pDeviceContext->GSSetConstantBuffers(1, 1, &mLightBuffer);
	return true;
}

bool DeferredShaderClass::SetCamConstantBufferParameters(ID3D11DeviceContext* pDeviceContext, CameraClass* pCamera)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	CamBuffer* dataPtr;
	unsigned int bufferNumber;

	// Lock the constant buffer so it can be written to.
	result = pDeviceContext->Map(mCamBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (CamBuffer*)mappedResource.pData;

	dataPtr->CamPos = pCamera->GetPosition();

	// Unlock the constant buffer.
	pDeviceContext->Unmap(mCamBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 1;

	// Set the constant buffer in the shader with the updated values.
	pDeviceContext->PSSetConstantBuffers(bufferNumber, 1, &mCamBuffer);

	return true;
}

bool DeferredShaderClass::SetVPConstantBufferParameters(ID3D11DeviceContext* pDeviceContext, CameraClass* pCamera)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VPBuffer* dataPtr;
	unsigned int bufferNumber;

	// Lock the constant buffer so it can be written to.
	result = pDeviceContext->Map(mVPBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (VPBuffer*)mappedResource.pData;

	XMMATRIX view = XMLoadFloat4x4(&pCamera->GetViewMatrix());
	XMMATRIX proj = XMLoadFloat4x4(&pCamera->GetProjMatrix());

	view = XMMatrixTranspose(view);
	proj = XMMatrixTranspose(proj);

	XMStoreFloat4x4(&dataPtr->view, view);
	XMStoreFloat4x4(&dataPtr->proj, proj);

	// Unlock the constant buffer.
	pDeviceContext->Unmap(mVPBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Set the constant buffer in the shader with the updated values.
	pDeviceContext->GSSetConstantBuffers(bufferNumber, 1, &mVPBuffer);

	return true;
}