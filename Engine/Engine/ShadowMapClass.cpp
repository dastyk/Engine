#include "ShadowMapClass.h"


ShadowMapClass::ShadowMapClass()
{
	mRTV = 0;
	mSRV = 0;
	mDSV = 0;
}


ShadowMapClass::~ShadowMapClass()
{
	if (mRTV)
	{
		mRTV->Release();
		mRTV = 0;
	}

	if (mSRV)
	{
		mSRV->Release();
		mSRV = 0;
	}
	if (mDSV)
	{
		mDSV->Release();
		mDSV = 0;
	}
	
}

bool ShadowMapClass::Init(ID3D11Device* pDevice, float w, float h)
{
	bool result;
	result = InitShader(pDevice, L"data/shaders/ShadowmapVS.hlsl", L"data/shaders/ShadowmapPS.hlsl", w, h);
	if (!result)
		return false;

	return true;
}

bool ShadowMapClass::CreateShadowMap(ID3D11DeviceContext* pDeviceContext, ObjectClass* pObject, PointLightClass* pPointLight)
{
	bool result;

	result = ShaderClass::SetConstantBufferParameters(pDeviceContext, pObject->GetWorldMatrix(), pPointLight->GetViewMatrix(), pPointLight->GetProjMatrix());
	if (!result)
		return false;


	// Set the constant buffer in the vertex shader with the updated values.
	pDeviceContext->VSSetConstantBuffers(0, 1, &mMatrixBuffer);


	

	RenderShader(pDeviceContext, pObject->GetIndexCount());

	

	return true;
}

void  ShadowMapClass::SetRTV(ID3D11DeviceContext* pDeviceContext)
{

	pDeviceContext->OMGetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, prevRTV, &prevDSV);
	pDeviceContext->OMSetRenderTargets(1, &mRTV, mDSV);

}
void  ShadowMapClass::ClearRTV(ID3D11DeviceContext* pDeviceContext)
{
	float color[] = { 0, 0, 0, 0 };

	pDeviceContext->ClearRenderTargetView(mRTV, color);
	pDeviceContext->ClearDepthStencilView(mDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
}
void  ShadowMapClass::UnbindRTV(ID3D11DeviceContext* pDeviceContext)
{
	pDeviceContext->OMSetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, prevRTV, prevDSV);
}

bool ShadowMapClass::InitShader(ID3D11Device* pDevice, WCHAR* vFile, WCHAR* pFile, float w, float h)
{
	bool result;
	HRESULT hr;

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

	result = ShaderClass::InitShader(pDevice, vFile, pFile, NULL, vertexDesc, numElements);
	if (!result)
	{
		return false;
	}

	D3D11_TEXTURE2D_DESC textureDesc;

	// Initialize the render target texture description.
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// Setup the render target texture description.
	textureDesc.Width = w;
	textureDesc.Height = h;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32_TYPELESS; //DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	ID3D11Texture2D* mTex;

	// Create the render target texture
	hr = pDevice->CreateTexture2D(&textureDesc, NULL, &mTex);
	if (FAILED(hr))
	{
		return false;
	}
	
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;

	// Setup the description of the render target view.
	renderTargetViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;


	// Create the render target view.
	hr = pDevice->CreateRenderTargetView(mTex, &renderTargetViewDesc, &mRTV);
	if (FAILED(hr))
	{
		return false;
	}
	
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	// Setup the description of the shader resource view.
	shaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.

	hr = pDevice->CreateShaderResourceView(mTex, &shaderResourceViewDesc, &mSRV);
	if (FAILED(hr))
	{
		return false;
	}
	

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = w;
	depthBufferDesc.Height = h;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;
	ID3D11Texture2D* mDSB;
	// Create the texture for the depth buffer using the filled out description.
	hr = pDevice->CreateTexture2D(&depthBufferDesc, NULL, &mDSB);
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
	hr = pDevice->CreateDepthStencilView(mDSB, &depthStencilViewDesc, &mDSV);
	if (FAILED(hr))
	{
		return false;
	}
	return true;

}

void ShadowMapClass::RenderShader(ID3D11DeviceContext* pDeviceContext, int indexCount)
{
	ShaderClass::RenderShader(pDeviceContext, indexCount);
}


ID3D11ShaderResourceView* ShadowMapClass::GetShaderResourceView()
{
	return mSRV;
}