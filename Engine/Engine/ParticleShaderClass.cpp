#include "ParticleShaderClass.h"


ParticleShaderClass::ParticleShaderClass()
{
	mSampleState = 0;
}


ParticleShaderClass::~ParticleShaderClass()
{
	if (mSampleState)
	{
		mSampleState->Release();
		mSampleState = 0;
	}
}

bool ParticleShaderClass::Init(ID3D11Device* pDevice)
{
	bool result;


	// Initialize the vertex and pixel shaders.
	result = InitShader(pDevice, L"data/shaders/ParticleVertexShader.hlsl", L"data/shaders/ParticleGeometryShader.hlsl", L"data/shaders/ParticlePixelShader.hlsl");
	if (!result)
	{
		return false;
	}

	return true;
}

bool ParticleShaderClass::InitShader(ID3D11Device* pDevice, WCHAR* vFileName, WCHAR* gFileName, WCHAR* pFileName)
{
	bool result;
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
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

	return true;
}

bool ParticleShaderClass::Render(ID3D11DeviceContext* pDeviceContext, ParticleSystemClass* mParticleS, CameraClass* pCamera)
{
	bool result;

	// Set the shader parameters that it will use for rendering.
	result = ParticleShaderClass::SetConstantBufferParameters(pDeviceContext, pCamera->GetViewMatrix(), pCamera->GetProjMatrix());
	if (!result)
	{
		return false;
	}

	// Set the constant buffer in the vertex shader with the updated values.
	pDeviceContext->GSSetConstantBuffers(0, 1, &mMatrixBuffer);

	TextureClass* pTexture = mParticleS->GetTexture();
	ID3D11ShaderResourceView** tex = pTexture->GetShaderResourceView();

	// Set shader texture resource in the pixel shader.
	pDeviceContext->PSSetShaderResources(0, pTexture->GetTextureCount(), tex);

	// Now render the prepared buffers with the shader.
	ParticleShaderClass::RenderShader(pDeviceContext, mParticleS->GetAliveParticles());

	return true;
}

bool ParticleShaderClass::SetConstantBufferParameters(ID3D11DeviceContext* pDeviceContext, XMFLOAT4X4& viewMatrix, XMFLOAT4X4& projMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ParticleConstBufferType* dataPtr;


	XMMATRIX view = XMLoadFloat4x4(&viewMatrix);
	XMMATRIX proj = XMLoadFloat4x4(&projMatrix);

	proj = XMMatrixTranspose(proj);
	view = XMMatrixTranspose(view);


	// Lock the constant buffer so it can be written to.
	result = pDeviceContext->Map(mMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}


	// Get a pointer to the data in the constant buffer.
	dataPtr = (ParticleConstBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.

	XMStoreFloat4x4(&dataPtr->Proj, proj);
	XMStoreFloat4x4(&dataPtr->View, view);


	// Unlock the constant buffer.
	pDeviceContext->Unmap(mMatrixBuffer, 0);



	return true;
}

void ParticleShaderClass::RenderShader(ID3D11DeviceContext* pDeviceContext, int vertexCount)
{
	// Set the vertex input layout.
	pDeviceContext->IASetInputLayout(mLayout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	pDeviceContext->VSSetShader(mVertexShader, nullptr, 0);
	pDeviceContext->HSSetShader(mHullShader, nullptr, 0);
	pDeviceContext->DSSetShader(mDomainShader, nullptr, 0);
	pDeviceContext->GSSetShader(mGeometryShader, nullptr, 0);
	pDeviceContext->PSSetShader(mPixelShader, nullptr, 0);

	// Render mesh stored in active buffers
	pDeviceContext->Draw(vertexCount, 0);

}