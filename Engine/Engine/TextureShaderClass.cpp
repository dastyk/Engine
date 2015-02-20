#include "TextureShaderClass.h"


TextureShaderClass::TextureShaderClass() : ShaderClass()
{
	mSampleState = 0;
}


TextureShaderClass::~TextureShaderClass() 
{
	if (mSampleState)
	{
		mSampleState->Release();
		mSampleState = 0;
	}
}

bool TextureShaderClass::Init(ID3D11Device* pDevice)
{
	bool result;


	// Initialize the vertex and pixel shaders.
	result = InitShader(pDevice, L"data/shaders/TexVertexShader.hlsl", L"data/shaders/TexPixelShader.hlsl", NULL);
	if (!result)
	{
		return false;
	}

	return true;
}


bool TextureShaderClass::InitShader(ID3D11Device* pDevice, WCHAR* vFileName, WCHAR* pFileName, WCHAR* gFileName)
{
	bool result;

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

	result = createSamplerState(pDevice,&mSampleState);
	if (!result)
	{
		return false;
	}

	return true;
}


bool TextureShaderClass::Render(ID3D11DeviceContext* pDeviceContext, int indexCount, XMFLOAT4X4& worldMatrix, XMFLOAT4X4& viewMatrix, XMFLOAT4X4& projMatrix, XMFLOAT3& dir, ID3D11ShaderResourceView* pTexture)
{
	bool result;




	// Set the shader parameters that it will use for rendering.
	result = ShaderClass::SetConstantBufferParameters(pDeviceContext, worldMatrix, viewMatrix, projMatrix,dir);
	if (!result)
	{
		return false;
	}

	pDeviceContext->VSSetConstantBuffers(0, 1, &mMatrixBuffer);

	// Set shader texture resource in the pixel shader.
	pDeviceContext->PSSetShaderResources(0, 1, &pTexture);
	
	// Now render the prepared buffers with the shader.
	RenderShader(pDeviceContext, indexCount);

	pDeviceContext->PSSetShaderResources(0, 1, &mNullSRW);

	return true;
}


void TextureShaderClass::RenderShader(ID3D11DeviceContext* pDeviceContext, int indexCount)
{

	pDeviceContext->PSSetSamplers(0, 1, &mSampleState);

	// Set parameters and render mesh
	ShaderClass::RenderShader(pDeviceContext, indexCount);
}

