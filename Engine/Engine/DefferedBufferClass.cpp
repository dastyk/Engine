#include "DefferedBufferClass.h"


DefferedBufferClass::DefferedBufferClass()
{
	int i;

	for (i = 0; i<BUFFER_COUNT; i++)
	{
		mRenderTargetTextureArray[i] = 0;
		mRenderTargetViewArray[i] = 0;
		mShaderResourceViewArray[i] = 0;
	}

}


DefferedBufferClass::~DefferedBufferClass()
{
	for (int i = 0; i<BUFFER_COUNT; i++)
	{
		if (mShaderResourceViewArray[i])
		{
			mShaderResourceViewArray[i]->Release();
			mShaderResourceViewArray[i] = 0;
		}

		if (mRenderTargetViewArray[i])
		{
			mRenderTargetViewArray[i]->Release();
			mRenderTargetViewArray[i] = 0;
		}

		if (mRenderTargetTextureArray[i])
		{
			mRenderTargetTextureArray[i]->Release();
			mRenderTargetTextureArray[i] = 0;
		}
	}

}

bool DefferedBufferClass::Init(ID3D11Device* device, int textureWidth, int textureHeight)
{
	HRESULT hr;

	D3D11_TEXTURE2D_DESC textureDesc;

	// Initialize the render target texture description.
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// Setup the render target texture description.
	textureDesc.Width = textureWidth;
	textureDesc.Height = textureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

		// Create the render target textures.
	for (int i = 0; i < BUFFER_COUNT; i++)
	{
		hr = device->CreateTexture2D(&textureDesc, NULL, &mRenderTargetTextureArray[i]);
		if (FAILED(hr))
		{
			return false;
		}
	}

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;

	// Setup the description of the render target view.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target views.
	for (int i = 0; i < BUFFER_COUNT; i++)
	{
		hr = device->CreateRenderTargetView(mRenderTargetTextureArray[i], &renderTargetViewDesc, &mRenderTargetViewArray[i]);
		if (FAILED(hr))
		{
			return false;
		}
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	// Setup the description of the shader resource view.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

		// Create the shader resource views.
	for (int i = 0; i < BUFFER_COUNT; i++)
	{
		hr = device->CreateShaderResourceView(mRenderTargetTextureArray[i], &shaderResourceViewDesc, &mShaderResourceViewArray[i]);
		if (FAILED(hr))
		{
			return false;
		}
	}

	return true;
}

void DefferedBufferClass::SetRenderTargets(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* pDSV)
{
	// Bind the render target view array and depth stencil buffer to the output render pipeline.
	deviceContext->OMSetRenderTargets(BUFFER_COUNT, mRenderTargetViewArray, pDSV);

}

void DefferedBufferClass::ClearRenderTargets(ID3D11DeviceContext* deviceContext, float red, float green, float blue, float alpha)
{
	float color[4];
	int i;


	// Setup the color to clear the buffer to.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear the render target buffers.
	for (i = 0; i<BUFFER_COUNT; i++)
	{
		deviceContext->ClearRenderTargetView(mRenderTargetViewArray[i], color);
	}
}

ID3D11ShaderResourceView* DefferedBufferClass::GetShaderResourceView(int view)
{
	return mShaderResourceViewArray[view];
}

ID3D11ShaderResourceView** DefferedBufferClass::GetShaderResourceView()
{
	return mShaderResourceViewArray;
}