#include "ComputeShaderClass.h"


ComputeShaderClass::ComputeShaderClass()
{
}


ComputeShaderClass::~ComputeShaderClass()
{
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


bool ComputeShaderClass::InitShaders(ID3D11Device* pDevice, WCHAR*)
{
	return true;
}

bool ComputeShaderClass::createComputeShader(ID3D11Device* pDevice)
{
	return true;
}