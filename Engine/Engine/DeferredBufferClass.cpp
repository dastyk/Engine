#include "DeferredBufferClass.h"


DeferredBufferClass::DeferredBufferClass()
{
	int i;

	for (i = 0; i<BUFFER_COUNT; i++)
	{
		mRenderTargetTextureArray[i] = 0;
		mRenderTargetViewArray[i] = 0;
		mShaderResourceViewArray[i] = 0;
	}
	mDepthStencilBuffer = 0;
	mDepthStencilView = 0;
}


DeferredBufferClass::~DeferredBufferClass()
{
	if (mDepthStencilBuffer)
	{
		mDepthStencilBuffer->Release();
		mDepthStencilBuffer = 0;
	}

	if (mDepthStencilView)
	{
		mDepthStencilView->Release();
		mDepthStencilView = 0;
	}


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

bool DeferredBufferClass::Init(ID3D11Device* device, int textureWidth, int textureHeight)
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
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; //DXGI_FORMAT_R8G8B8A8_UNORM;
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

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = textureWidth;
	depthBufferDesc.Height = textureHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	hr = device->CreateTexture2D(&depthBufferDesc, NULL, &mDepthStencilBuffer);
	if (FAILED(hr))
	{
		return false;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	// Initailze the depth stencil view description.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	hr = device->CreateDepthStencilView(mDepthStencilBuffer, &depthStencilViewDesc, &mDepthStencilView);
	if (FAILED(hr))
	{
		return false;
	}

	// Setup the viewport for rendering.
	mViewport.Width = (float)textureWidth;
	mViewport.Height = (float)textureHeight;
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;
	mViewport.TopLeftX = 0.0f;
	mViewport.TopLeftY = 0.0f;


	return true;
}

void DeferredBufferClass::SetRenderTargets(ID3D11DeviceContext* deviceContext)
{
	for (int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
	{
		prevRTV[i] = nullptr;
	}

	deviceContext->OMGetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, prevRTV, &prevDSV);

	// Bind the render target view array and depth stencil buffer to the output render pipeline.
	deviceContext->OMSetRenderTargets(BUFFER_COUNT, mRenderTargetViewArray, mDepthStencilView);

	// Set the viewport.
	deviceContext->RSSetViewports(1, &mViewport);
}

void DeferredBufferClass::UnsetRenderTargets(ID3D11DeviceContext* deviceContext)
{
	deviceContext->OMSetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, prevRTV, prevDSV);
}

void DeferredBufferClass::ClearRenderTargets(ID3D11DeviceContext* deviceContext, float red, float green, float blue, float alpha)
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

	// Clear the depth buffer.
	deviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

ID3D11ShaderResourceView* DeferredBufferClass::GetShaderResourceView(int view)
{
	return mShaderResourceViewArray[view];
}

ID3D11ShaderResourceView** DeferredBufferClass::GetShaderResourceView()
{
	return mShaderResourceViewArray;
}