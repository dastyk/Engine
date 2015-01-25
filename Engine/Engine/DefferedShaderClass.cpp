#include "DefferedShaderClass.h"


DefferedShaderClass::DefferedShaderClass() : ShaderClass()
{
	mSampleState = 0;
}


DefferedShaderClass::~DefferedShaderClass()
{
	if (mSampleState)
	{
		mSampleState->Release();
		mSampleState = 0;
	}
}

bool DefferedShaderClass::Init(ID3D11Device* pDevice)
{
	bool result;


	// Initialize the vertex and pixel shaders.
	result = InitShader(pDevice, L"data/shaders/DefferedVertexShader.hlsl", L"data/shaders/DefferedPixelShader.hlsl", NULL,
		L"data/shaders/CompleDefferedVertexShader.hlsl", L"data/shaders/CompleDefferedPixelShader.hlsl", L"data/shaders/LightGeometryShader.hlsl");
	if (!result)
	{
		return false;
	}

	return true;
}

bool DefferedShaderClass::RenderDeffered(ID3D11DeviceContext* pDeviceContext, ObjectClass* pObject, CameraClass* pCamera)
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

bool DefferedShaderClass::Render(ID3D11DeviceContext* pDeviceContext, DefferedBufferClass* pBuffer)
{
	bool result;
	unsigned int bufferNumber;

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Set the constant buffer in the vertex shader with the updated values.
	pDeviceContext->VSSetConstantBuffers(bufferNumber, 1, &mMatrixBuffer);

	ID3D11ShaderResourceView** tex = pBuffer->GetShaderResourceView();

	// Set shader texture resource in the pixel shader.
	pDeviceContext->PSSetShaderResources(0, BUFFER_COUNT, tex);

	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	// Now render the prepared buffers with the shader.
	RenderShader(pDeviceContext, 1);

	return true;
}



bool DefferedShaderClass::InitShader(ID3D11Device* pDevice, WCHAR* dVertex, WCHAR* dPixel, WCHAR* dGeometry, WCHAR* vertex, WCHAR* pixel, WCHAR* geometry)
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

	result = ShaderClass::InitShader(pDevice, dVertex, dPixel, dGeometry, vertexDesc, numElements);
	if (!result)
	{
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC vertexDesc2[] =
	{
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// Get a count of the elements in the layout.
	numElements = sizeof(vertexDesc2) / sizeof(vertexDesc2[0]);

	result = createVertexShaderAndInputLayout(pDevice, vertex, "VSMain", NULL, 0);
	if (!result)
	{
		return false;
	}

	result = createPixelShader(pDevice, pixel, "PSMain");
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

	result = createSamplerState(pDevice, &mSampleState);
	if (!result)
	{
		return false;
	}

	return true;
}

void DefferedShaderClass::RenderShader(ID3D11DeviceContext* pDeviceContext, int indexCount)
{
	pDeviceContext->PSSetSamplers(0, 1, &mSampleState);

	// Set parameters and render mesh
	ShaderClass::RenderShader(pDeviceContext, indexCount);
}
