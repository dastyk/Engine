#include "ComputeShaderClass.h"


ComputeShaderClass::ComputeShaderClass()
{
	mComputeShader = 0;
}


ComputeShaderClass::~ComputeShaderClass()
{
	if (mComputeShader)
	{
		mComputeShader->Release();
		mComputeShader = 0;
	}
}

void ComputeShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, WCHAR* shaderFilename)
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

bool ComputeShaderClass::Init(ID3D11Device* pDevice)
{

	return true;
}


bool ComputeShaderClass::InitShaders(ID3D11Device* pDevice, WCHAR* cName)
{
	bool result;

	result = createComputeShader(pDevice, cName, "CSMain");
	if (!result)
		return false;

	return true;
}

bool ComputeShaderClass::createComputeShader(ID3D11Device* pDevice, WCHAR* cName, char* EntryPoint)
{
	HRESULT hr;
	// Initialie Geometry shader
	ID3D10Blob* computeShaderBuffer;
	ID3D10Blob* errorMessages;
	hr = D3DCompileFromFile(
		cName,
		NULL,
		NULL,
		EntryPoint,
		"cs_5_0",
		0,
		0,
		&computeShaderBuffer,
		&errorMessages);

	if (FAILED(hr))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessages)
		{
			OutputShaderErrorMessage(errorMessages, cName);
			return false;
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(0, cName, L"Missing Shader File", MB_OK);
			return false;
		}
	}

	// Create the geometry shader from the buffer.
	hr = pDevice->CreateComputeShader(computeShaderBuffer->GetBufferPointer(), computeShaderBuffer->GetBufferSize(), NULL, &mComputeShader);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to create compute shader", 0, MB_OK);
		return false;
	}

	computeShaderBuffer->Release();



	return true;
}

bool ComputeShaderClass::Compute(ID3D11DeviceContext* pDeviceContext, UINT ThreadGroupCountX, UINT ThreadGroupCountY)
{

	pDeviceContext->CSSetShader(mComputeShader, NULL, 0);

	pDeviceContext->Dispatch(ThreadGroupCountX, ThreadGroupCountY, 1);

	pDeviceContext->CSSetShader(NULL, NULL, 0);

	return true;
}