#include "LightShaderClass.h"


LightShaderClass::LightShaderClass() : ShaderClass()
{
	mSampleState = 0;
	mLightBuffer = 0;
	mBoneTBuffer = 0;
	mAniGS = 0;
}


LightShaderClass::~LightShaderClass()
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
	if (mBoneTBuffer)
	{
		mBoneTBuffer->Release();
		mBoneTBuffer = 0;
	}
	if (mAniGS)
	{
		mAniGS->Release();
		mAniGS = 0;
	}
}

bool LightShaderClass::Init(ID3D11Device* pDevice)
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


bool LightShaderClass::InitShader(ID3D11Device* pDevice, WCHAR* vFileName, WCHAR* pFileName, WCHAR* gFileName)
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
		{ "BLENDINDICES", 1, DXGI_FORMAT_R32_UINT, 0, 64, D3D11_INPUT_PER_VERTEX_DATA, 0 }
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

	result = createConstantBuffer(pDevice, sizeof(XMFLOAT4) * 3 + sizeof(PointLight)*MAX_ACTIVE_LIGHTS + sizeof(MatrialDescPadded)*MAX_MATERIAL_COUNT, &mLightBuffer, D3D11_BIND_CONSTANT_BUFFER);
	if (!result)
	{
		return false;
	}

	result = createConstantBuffer(pDevice, sizeof(XMFLOAT4X4)* 2 + sizeof(XMFLOAT4X4)*MAX_BONE_COUNT, &mBoneTBuffer, D3D11_BIND_CONSTANT_BUFFER);
	if (!result)
	{
		return false;
	}


	result = createGeometryShader(pDevice, L"data/shaders/AnimatedLightGeometryShader.hlsl", "GSMain", &mAniGS);
	if (!result)
	{
		return false;
	}


	return true;
}


bool LightShaderClass::Render(ID3D11DeviceContext* pDeviceContext, ObjectClass* pObject, CameraClass* pCamera, PointLightClass** ppLights, UINT NrOfLights, FogClass* pDrawDistFog)
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

	result = SetConstantBufferParameters(pDeviceContext, ppLights, NrOfLights, pObject, pDrawDistFog, pCamera);
	if (!result)
	{
		return false;
	}

	TextureClass* pTexture = pObject->GetTexture();
	ID3D11ShaderResourceView** tex = pTexture->GetShaderResourceView();

	// Set shader texture resource in the pixel shader.
	pDeviceContext->PSSetShaderResources(0,pTexture->GetTextureCount(), tex);

	// Now render the prepared buffers with the shader.
	RenderShader(pDeviceContext, pObject->GetIndexCount(0));

	return true;
}


void LightShaderClass::RenderShader(ID3D11DeviceContext* pDeviceContext, int indexCount)
{

	pDeviceContext->PSSetSamplers(0, 1, &mSampleState);

	// Set parameters and render mesh
	ShaderClass::RenderShader(pDeviceContext, indexCount);
}

bool LightShaderClass::SetConstantBufferParameters(ID3D11DeviceContext* pDeviceContext, PointLightClass** ppLights, UINT NrOfLights, ObjectClass* pObject, FogClass* pDrawDistFog, CameraClass* pCamera)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	LightConstantBuffer* dataPtr;
	unsigned int bufferNumber;

	// Lock the constant buffer so it can be written to.
	result = pDeviceContext->Map(mLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (LightConstantBuffer*)mappedResource.pData;

	ModelClass* pModel = pObject->GetModel();
	MatrialDesc *pMaterials = pModel->GetMaterials();
	int oCount = pModel->GetObjectCount();

	dataPtr->LightCount_FogRange_ObjectCount_Unused = XMFLOAT4(NrOfLights, pDrawDistFog->GetRange(), oCount, 0);
	dataPtr->fogColor = pDrawDistFog->GetColor();
	dataPtr->CamPos = pCamera->GetPosition();

	for (int i = 0; i < NrOfLights; i++)
	{
		dataPtr->lights[i].Pos = ppLights[i]->GetLightPos();
		XMFLOAT3 color = ppLights[i]->GetLightColor();
		dataPtr->lights[i].Color_LightRange = XMFLOAT4(color.x, color.y, color.z, ppLights[i]->GetRadius());
	}
	
	for (int i = 0; i < oCount; i++)
	{
		dataPtr->materials[i].Ambient = pMaterials[i].Ambient;
		dataPtr->materials[i].Diffuse = pMaterials[i].Diffuse;
		dataPtr->materials[i].Specular = pMaterials[i].Specular;
		dataPtr->materials[i].Reflectivity = pMaterials[i].Reflectivity;

		dataPtr->materials[i].SpecPower_AlphaClip_Unused_Unused = XMFLOAT4(pMaterials[i].SpecPower, pMaterials[i].AlphaClip, 0, 0);
	}

	// Unlock the constant buffer.
	pDeviceContext->Unmap(mLightBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Set the constant buffer in the shader with the updated values.
	pDeviceContext->PSSetConstantBuffers(bufferNumber, 1, &mLightBuffer);

	return true;
}

bool LightShaderClass::RenderAnimated(ID3D11DeviceContext* pDeviceContext, ObjectClass* pObject, CameraClass* pCamera, PointLightClass** ppLights, UINT NrOfLights, FogClass* pDrawDistFog)
{
	bool result;
	unsigned int bufferNumber;

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	result = SetTbufferParameters(pDeviceContext, pObject->GetWorldMatrix(), pCamera->GetViewMatrix(), pCamera->GetProjMatrix(), pObject);
	if (!result)
	{
		return false;
	}
	pDeviceContext->GSSetConstantBuffers(0, 1, &mBoneTBuffer);

	result = SetConstantBufferParameters(pDeviceContext, ppLights, NrOfLights, pObject, pDrawDistFog, pCamera);
	if (!result)
	{
		return false;
	}

	

	TextureClass* pTexture = pObject->GetTexture();
	ID3D11ShaderResourceView** tex = pTexture->GetShaderResourceView();

	// Set shader texture resource in the pixel shader.
	pDeviceContext->PSSetShaderResources(0, pTexture->GetTextureCount(), tex);

	// Now render the prepared buffers with the shader.
	pDeviceContext->PSSetSamplers(0, 1, &mSampleState);

	// Set the vertex input layout.
	pDeviceContext->IASetInputLayout(mLayout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	pDeviceContext->VSSetShader(mVertexShader, nullptr, 0);
	pDeviceContext->HSSetShader(mHullShader, nullptr, 0);
	pDeviceContext->DSSetShader(mDomainShader, nullptr, 0);
	pDeviceContext->GSSetShader(mAniGS, nullptr, 0);
	pDeviceContext->PSSetShader(mPixelShader, nullptr, 0);

	// Render mesh stored in active buffers
	pDeviceContext->DrawIndexed(pObject->GetIndexCount(0), 0, 0);

	return true;
}

bool LightShaderClass::SetTbufferParameters(ID3D11DeviceContext* pDeviceContext, XMFLOAT4X4& worldMatrix, XMFLOAT4X4& viewMatrix, XMFLOAT4X4& projMatrix, ObjectClass* pObject)
{

	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	LMatrixBufferType* dataPtr;


	XMMATRIX world = XMLoadFloat4x4(&worldMatrix);
	XMMATRIX view = XMLoadFloat4x4(&viewMatrix);
	XMMATRIX proj = XMLoadFloat4x4(&projMatrix);


	XMMATRIX mWorldViewProj = world*view*proj;
	XMMATRIX mWorld = world;

	mWorldViewProj = XMMatrixTranspose(mWorldViewProj);
	mWorld = XMMatrixTranspose(mWorld);

	// Lock the constant buffer so it can be written to.
	result = pDeviceContext->Map(mBoneTBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}


	// Get a pointer to the data in the constant buffer.
	dataPtr = (LMatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.

	XMStoreFloat4x4(&dataPtr->mWorldViewProj, mWorldViewProj);
	XMStoreFloat4x4(&dataPtr->mWorld, mWorld);

	XMFLOAT4X4* m;
	pObject->Animate(&m);
	UINT count = pObject->GetModel()->GetBoneCount();

	for (UINT i = 0; i < count; i++)
	{
		//XMStoreFloat4x4(&dataPtr->mBones[i], XMMatrixIdentity());
		dataPtr->mBones[i] = m[i];
	}
	

	// Unlock the constant buffer.
	pDeviceContext->Unmap(mBoneTBuffer, 0);


	delete[] m;

	return true;
}
