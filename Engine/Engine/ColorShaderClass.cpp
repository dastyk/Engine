#include "ColorShaderClass.h"


ColorShaderClass::ColorShaderClass() : ShaderClass()
{

}


ColorShaderClass::~ColorShaderClass()
{

}

bool ColorShaderClass::Init(ID3D11Device* pDevice)
{
	bool result;


	// Initialize the vertex and pixel shaders.
	result = InitShader(pDevice, L"data/shaders/ColorVertexShader.hlsl", L"data/shaders/ColorPixelShader.hlsl");
	if (!result)
	{
		return false;
	}

	return true;
}

bool ColorShaderClass::InitShader(ID3D11Device* pDevice, WCHAR* vFileName, WCHAR* pFileName)
{
	bool result;
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// Get a count of the elements in the layout.
	int numElements = sizeof(vertexDesc) / sizeof(vertexDesc[0]);

	result = ShaderClass::InitShader(pDevice, vFileName, pFileName,NULL, vertexDesc, numElements);
	if (!result)
	{
		return false;
	}

	return true;
}

bool ColorShaderClass::Render(ID3D11DeviceContext* pDeviceContext, int indexCount, XMFLOAT4X4& worldMatrix, XMFLOAT4X4& viewMatrix, XMFLOAT4X4& projMatrix, XMFLOAT3& dir)
{
	bool result;

	
	// Set the shader parameters that it will use for rendering.
	result = ShaderClass::SetConstantBufferParameters(pDeviceContext, worldMatrix, viewMatrix, projMatrix, dir);
	if (!result)
	{
		return false;
	}

	// Now render the prepared buffers with the shader.
	RenderShader(pDeviceContext, indexCount);

	return true;
}



void ColorShaderClass::RenderShader(ID3D11DeviceContext* pDeviceContext, int indexCount)
{



	// Set parameters and render mesh
	ShaderClass::RenderShader(pDeviceContext, indexCount);
}