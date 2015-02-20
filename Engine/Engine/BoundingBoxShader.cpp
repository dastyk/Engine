#include "BoundingBoxShader.h"


BoundingBoxShader::BoundingBoxShader()
{
}


BoundingBoxShader::~BoundingBoxShader()
{
}

bool BoundingBoxShader::Init(ID3D11Device* pDevice)
{
	bool result;


	// Initialize the vertex and pixel shaders.
	result = InitShader(pDevice, L"data/shaders/BoundingBoxVS.hlsl", L"data/shaders/BoudingBoxPS.hlsl");
	if (!result)
	{
		return false;
	}

	return true;

}

bool BoundingBoxShader::Render(ID3D11DeviceContext* pDeviceContext, ObjectClass* pObject, CameraClass* pCamera)
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
	pDeviceContext->VSSetConstantBuffers(bufferNumber, 1, &mMatrixBuffer);

	// Now render the prepared buffers with the shader.
	RenderShader(pDeviceContext, 36);

	return true;
}



bool BoundingBoxShader::InitShader(ID3D11Device* pDevice, WCHAR* vertex, WCHAR* pixel)
{
	bool result;
	HRESULT hr;
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};


	// Get a count of the elements in the layout.
	int numElements = sizeof(vertexDesc) / sizeof(vertexDesc[0]);

	result = ShaderClass::InitShader(pDevice, vertex, pixel, NULL, vertexDesc, numElements);
	if (!result)
	{
		return false;
	}

}


void BoundingBoxShader::RenderShader(ID3D11DeviceContext* pDeviceContext, int indexCount)
{
	ShaderClass::RenderShader(pDeviceContext, indexCount);
}

