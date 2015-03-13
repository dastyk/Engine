#include "ShaderClass.h"


ShaderClass::ShaderClass()
{
	mVertexShader = 0;
	mPixelShader = 0;
	mGeometryShader = 0;
	mHullShader = 0;
	mDomainShader = 0;
	mComputeShader = 0;
	mLayout = 0;
	mMatrixBuffer = 0;
	
}


ShaderClass::~ShaderClass()
{
	
	// Release the matrix constant buffer.
	if (mMatrixBuffer)
	{
		mMatrixBuffer->Release();
		mMatrixBuffer = 0;
	}
	// Release the layout.
	if (mLayout)
	{
		mLayout->Release();
		mLayout = 0;
	}

	// Release the pixel shader.
	if (mPixelShader)
	{
		mPixelShader->Release();
		mPixelShader = 0;
	}

	// Release the vertex shader.
	if (mVertexShader)
	{
		mVertexShader->Release();
		mVertexShader = 0;
	}
	if (mGeometryShader)
	{
		mGeometryShader->Release();
		mGeometryShader = 0;
	}
	if (mHullShader)
	{
		mHullShader->Release();
		mHullShader = 0;
	}
	if (mDomainShader)
	{
		mDomainShader->Release();
		mDomainShader = 0;
	}
	if (mComputeShader)
	{
		mComputeShader->Release();
		mComputeShader = 0;
	}
}


void ShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	std::ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(0, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}

bool ShaderClass::Init(ID3D11Device* pDevice)
{
	return true;
}


void ShaderClass::RenderShader(ID3D11DeviceContext* pDeviceContext, int indexCount)
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
	pDeviceContext->DrawIndexed(indexCount, 0, 0);
	return;
}

bool ShaderClass::createVertexShaderAndInputLayout(ID3D11Device* pDevice, WCHAR* fileName, CHAR* EntryPoint, D3D11_INPUT_ELEMENT_DESC *vertexDesc, int numElements)
{
	HRESULT hr;
	bool result;
	// Initialize the vertex shader
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* errorMessages;

	hr = D3DCompileFromFile(
		fileName,
		NULL,
		NULL,
		EntryPoint,
		"vs_5_0",
		0,
		0,
		&vertexShaderBuffer,
		&errorMessages);

	if (FAILED(hr))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessages)
		{
			OutputShaderErrorMessage(errorMessages, fileName);
			return false;
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(0, fileName, L"Missing Shader File", MB_OK);
			return false;
		}
	}

	// Create the vertex shader from the buffer.
	hr = pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &mVertexShader);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to create vertex shader", 0, MB_OK);
		return false;
	}

	if (FAILED(hr))
	{
		MessageBox(0, L"Could not create VertexShader.", 0, 0);
		return false;
	}

	result = createInputLayout(pDevice,vertexDesc, vertexShaderBuffer, numElements);
	if (!result)
	{
		return false;
	}

	vertexShaderBuffer->Release();
	return true;
}
bool ShaderClass::createPixelShader(ID3D11Device* pDevice, WCHAR* fileName, CHAR* EntryPoint)
{
	HRESULT hr;
	// Initialie Pixel shader
	ID3D10Blob* pixelShaderBuffer;
	ID3D10Blob* errorMessages;
	hr = D3DCompileFromFile(
		fileName,
		NULL,
		NULL,
		EntryPoint,
		"ps_5_0",
		0,
		0,
		&pixelShaderBuffer,
		&errorMessages);

	if (FAILED(hr))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessages)
		{
			OutputShaderErrorMessage(errorMessages, fileName);
			return false;
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(0, fileName, L"Missing Shader File", MB_OK);
			return false;
		}
	}

	// Create the pixel shader from the buffer.
	hr = pDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &mPixelShader);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to create pixel shader", 0, MB_OK);
		return false;
	}

	pixelShaderBuffer->Release();

	return true;
}

bool ShaderClass::createInputLayout(ID3D11Device* pDevice, D3D11_INPUT_ELEMENT_DESC *vertexDesc, ID3D10Blob* pVertexShaderBuffer, int numElements)
{
	HRESULT hr;
	
	hr = pDevice->CreateInputLayout(
		vertexDesc,
		numElements,
		pVertexShaderBuffer->GetBufferPointer(),
		pVertexShaderBuffer->GetBufferSize(),
		&mLayout);

	if (FAILED(hr))
	{
		MessageBox(0, L"Could not create input layout.", 0, 0);
		return false;
	}
	return true;
}


bool ShaderClass::Render(ID3D11DeviceContext* pDeviceContext, ParticleSystemClass* mParticleS, CameraClass* pCamera)
{


	return true;
}

bool ShaderClass::Render(ID3D11DeviceContext* pDeviceContext, int indexCount, XMFLOAT4X4& worldMatrix, XMFLOAT4X4& viewMatrix, XMFLOAT4X4& projMatrix)
{
	

	return true;
}

bool ShaderClass::Render(ID3D11DeviceContext* pDeviceContext, int indexCount, XMFLOAT4X4& worldMatrix, XMFLOAT4X4& viewMatrix, XMFLOAT4X4& projMatrix, ID3D11ShaderResourceView* pTexture)
{

	return true;
}

bool ShaderClass::Render(ID3D11DeviceContext* pDeviceContext, ObjectClass* pObject, CameraClass* pCamera, PointLightClass** ppLights, UINT NrOfLights, FogClass* pDrawDistFog)
{
	return true;
}

bool ShaderClass::SetConstantBufferParameters(ID3D11DeviceContext* pDeviceContext, XMFLOAT4X4& worldMatrix , XMFLOAT4X4& viewMatrix, XMFLOAT4X4& projMatrix, XMFLOAT3& direction)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;


	XMMATRIX world = XMLoadFloat4x4(&worldMatrix);
	XMMATRIX view = XMLoadFloat4x4(&viewMatrix);
	XMMATRIX proj = XMLoadFloat4x4(&projMatrix);


	XMMATRIX mWorldViewProj = world*view*proj;
	XMMATRIX mWorld = world;

	mWorldViewProj = XMMatrixTranspose(mWorldViewProj);	
	mWorld = XMMatrixTranspose(mWorld);

	// Lock the constant buffer so it can be written to.
	result = pDeviceContext->Map(mMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	
	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.

	XMStoreFloat4x4(&dataPtr->mWorldViewProj, mWorldViewProj);
	XMStoreFloat4x4(&dataPtr->mWorld, mWorld);
	dataPtr->camPos = XMFLOAT4(direction.x, direction.y, direction.z, 0);


	// Unlock the constant buffer.
	pDeviceContext->Unmap(mMatrixBuffer, 0);

	

	return true;
}

bool ShaderClass::InitShader(ID3D11Device* pDevice, WCHAR* vFileName, WCHAR* pFileName, WCHAR* gFileName, D3D11_INPUT_ELEMENT_DESC *vertexDesc, int numElements)
{
	bool result;

	// Initialize the vertex shader
	result = createVertexShaderAndInputLayout(pDevice, vFileName, "VSMain", vertexDesc, numElements);
	if (!result)
	{
		return false;
	}

	if (gFileName)
	{
		// Init Geometry Shader
		result = createGeometryShader(pDevice, gFileName, "GSMain");
		if (!result)
		{
			return false;
		}
	}

	if (pFileName)
	{
		// Initialie Pixel shader
		result = createPixelShader(pDevice, pFileName, "PSMain");
		if (!result)
		{
			return false;
		}
	}
	
	

	result = createConstantBuffer(pDevice, sizeof(MatrixBufferType), &mMatrixBuffer, D3D11_BIND_CONSTANT_BUFFER);
	if (!result)
	{
		return false;
	}

	return true;
}

bool ShaderClass::createGeometryShader(ID3D11Device* pDevice, WCHAR* fileName, CHAR* EntryPoint)
{

	HRESULT hr;
	// Initialie Geometry shader
	ID3D10Blob* geometryShaderBuffer;
	ID3D10Blob* errorMessages;
	hr = D3DCompileFromFile(
		fileName,
		NULL,
		NULL,
		EntryPoint,
		"gs_5_0",
		0,
		0,
		&geometryShaderBuffer,
		&errorMessages);

	if (FAILED(hr))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessages)
		{
			OutputShaderErrorMessage(errorMessages, fileName);
			return false;
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(0, fileName, L"Missing Shader File", MB_OK);
			return false;
		}
	}

	// Create the geometry shader from the buffer.
	hr = pDevice->CreateGeometryShader(geometryShaderBuffer->GetBufferPointer(), geometryShaderBuffer->GetBufferSize(), NULL, &mGeometryShader);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to create geometry shader", 0, MB_OK);
		return false;
	}

	geometryShaderBuffer->Release();

	return true;
}

bool ShaderClass::createSamplerState(ID3D11Device* pDevice, ID3D11SamplerState** ppSampleState)
{
	HRESULT hr;
	D3D11_SAMPLER_DESC samplerDesc;

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;//D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	hr = pDevice->CreateSamplerState(&samplerDesc, ppSampleState);
	if (FAILED(hr))
	{
		MessageBox(0, L"Could not create Sampler State.", 0, 0);
		return false;
	}

	return true;
}

bool ShaderClass::createSamplerState(ID3D11Device* pDevice, ID3D11SamplerState** ppSampleState, D3D11_SAMPLER_DESC* samplerDesc)
{
	HRESULT hr;

	// Create the texture sampler state.
	hr = pDevice->CreateSamplerState(samplerDesc, ppSampleState);
	if (FAILED(hr))
	{
		MessageBox(0, L"Could not create Sampler State.", 0, 0);
		return false;
	}

	return true;
}

bool ShaderClass::createConstantBuffer(ID3D11Device* pDevice, UINT byteWidth, ID3D11Buffer** ppBuffer, UINT bindFlags)
{
	HRESULT hr;

	// Setup the description of the dynamic matrix constant buffer.
	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = byteWidth;
	matrixBufferDesc.BindFlags = bindFlags;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA sub;
	XMFLOAT4X4 d;
	XMStoreFloat4x4(&d, XMMatrixIdentity());
	sub.pSysMem = (void*)&d;

	sub.SysMemPitch = 0;

	sub.SysMemSlicePitch = 0;

	// Create the constant buffer pointer so we can access the constant buffer from within this class.
	hr = pDevice->CreateBuffer(&matrixBufferDesc, &sub, ppBuffer);
	if (FAILED(hr))
	{
		MessageBox(0, L"Could not create constant buffer.", 0, 0);
		return false;
	}

	return true;
}




bool ShaderClass::createVertexShaderAndInputLayout(ID3D11Device* pDevice, WCHAR* fileName, CHAR* EntryPoint, D3D11_INPUT_ELEMENT_DESC *vertexDesc, int numElements, ID3D11VertexShader** ppVshader, ID3D11InputLayout** ppLayout)
{
	HRESULT hr;
	bool result;
	// Initialize the vertex shader
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* errorMessages;

	hr = D3DCompileFromFile(
		fileName,
		NULL,
		NULL,
		EntryPoint,
		"vs_5_0",
		0,
		0,
		&vertexShaderBuffer,
		&errorMessages);

	if (FAILED(hr))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessages)
		{
			OutputShaderErrorMessage(errorMessages, fileName);
			return false;
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(0, fileName, L"Missing Shader File", MB_OK);
			return false;
		}
	}

	// Create the vertex shader from the buffer.
	hr = pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, ppVshader);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to create vertex shader", 0, MB_OK);
		return false;
	}

	if (FAILED(hr))
	{
		MessageBox(0, L"Could not create VertexShader.", 0, 0);
		return false;
	}

	result = createInputLayout(pDevice, vertexDesc, vertexShaderBuffer, numElements, ppLayout);
	if (!result)
	{
		return false;
	}

	vertexShaderBuffer->Release();
	return true;
}
bool ShaderClass::createPixelShader(ID3D11Device* pDevice, WCHAR* fileName, CHAR* EntryPoint, ID3D11PixelShader** ppPShader)
{
	HRESULT hr;
	// Initialie Pixel shader
	ID3D10Blob* pixelShaderBuffer;
	ID3D10Blob* errorMessages;
	hr = D3DCompileFromFile(
		fileName,
		NULL,
		NULL,
		EntryPoint,
		"ps_5_0",
		0,
		0,
		&pixelShaderBuffer,
		&errorMessages);

	if (FAILED(hr))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessages)
		{
			OutputShaderErrorMessage(errorMessages, fileName);
			return false;
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(0, fileName, L"Missing Shader File", MB_OK);
			return false;
		}
	}

	// Create the pixel shader from the buffer.
	hr = pDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, ppPShader);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to create pixel shader", 0, MB_OK);
		return false;
	}

	pixelShaderBuffer->Release();

	return true;
}


bool ShaderClass::createGeometryShader(ID3D11Device* pDevice, WCHAR* fileName, CHAR* EntryPoint, ID3D11GeometryShader** ppGShader)
{

	HRESULT hr;
	// Initialie Geometry shader
	ID3D10Blob* geometryShaderBuffer;
	ID3D10Blob* errorMessages;
	hr = D3DCompileFromFile(
		fileName,
		NULL,
		NULL,
		EntryPoint,
		"gs_5_0",
		0,
		0,
		&geometryShaderBuffer,
		&errorMessages);

	if (FAILED(hr))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessages)
		{
			OutputShaderErrorMessage(errorMessages, fileName);
			return false;
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(0, fileName, L"Missing Shader File", MB_OK);
			return false;
		}
	}

	// Create the geometry shader from the buffer.
	hr = pDevice->CreateGeometryShader(geometryShaderBuffer->GetBufferPointer(), geometryShaderBuffer->GetBufferSize(), NULL, ppGShader);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to create geometry shader", 0, MB_OK);
		return false;
	}

	geometryShaderBuffer->Release();

	return true;
}


bool ShaderClass::createInputLayout(ID3D11Device* pDevice, D3D11_INPUT_ELEMENT_DESC *vertexDesc, ID3D10Blob* pVertexShaderBuffer, int numElements, ID3D11InputLayout** ppLayout)
{
	HRESULT hr;

	hr = pDevice->CreateInputLayout(
		vertexDesc,
		numElements,
		pVertexShaderBuffer->GetBufferPointer(),
		pVertexShaderBuffer->GetBufferSize(),
		ppLayout);

	if (FAILED(hr))
	{
		MessageBox(0, L"Could not create input layout.", 0, 0);
		return false;
	}
	return true;
}

bool ShaderClass::createVertexShader(ID3D11Device* pDevice, WCHAR* fileName, CHAR* EntryPoint, ID3D11VertexShader** ppVShader)
{
	HRESULT hr;
	// Initialie Pixel shader
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* errorMessages;
	hr = D3DCompileFromFile(
		fileName,
		NULL,
		NULL,
		EntryPoint,
		"vs_5_0",
		0,
		0,
		&vertexShaderBuffer,
		&errorMessages);

	if (FAILED(hr))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessages)
		{
			OutputShaderErrorMessage(errorMessages, fileName);
			return false;
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(0, fileName, L"Missing Shader File", MB_OK);
			return false;
		}
	}

	// Create the pixel shader from the buffer.
	hr = pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, ppVShader);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to create vertex shader", 0, MB_OK);
		return false;
	}

	vertexShaderBuffer->Release();

	return true;
}
