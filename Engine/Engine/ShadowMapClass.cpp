#include "ShadowMapClass.h"


ShadowMapClass::ShadowMapClass()
{

	mSRV = 0;
	mDSV = 0;
	mDSB = 0;
	mTex = 0;
	mDepthStencilState = 0;
}


ShadowMapClass::~ShadowMapClass()
{


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
	if (mDSB)
	{
		mDSB->Release();
		mDSB = 0;
	}
	if (mTex)
	{
		mTex->Release();
		mTex = 0;
	}
	if (mDepthStencilState)
	{
		mDepthStencilState->Release();
		mDepthStencilState = 0;
	}
	
}

bool ShadowMapClass::Init(ID3D11Device* pDevice, float w, float h)
{
	bool result;
	result = InitShader(pDevice, L"data/shaders/ShadowmapVS.hlsl", NULL, NULL, w, h);// L"data/shaders/ShadowmapPS.hlsl", L"data/shaders/ShadowmapGS.hlsl", w, h);
	if (!result)
		return false;

	return true;
}

bool ShadowMapClass::CreateShadowMap(ID3D11DeviceContext* pDeviceContext, ObjectClass* pObject, PointLightClass* pPointLight)
{
	bool result;

	result = ShaderClass::SetConstantBufferParameters(pDeviceContext, pObject->GetWorldMatrix(), pPointLight->GetViewMatrix(), pPointLight->GetProjMatrix(), pPointLight->GetLightDir());
	if (!result)
		return false;


	// Set the constant buffer in the vertex shader with the updated values.
	pDeviceContext->VSSetConstantBuffers(0, 1, &mMatrixBuffer);




	RenderShader(pDeviceContext, pObject->GetIndexCount(0));

	

	return true;
}

void  ShadowMapClass::SetRTV(ID3D11DeviceContext* pDeviceContext)
{
	pDeviceContext->OMSetRenderTargets(0, NULL, mDSV);

	// Set the depth stencil state.
	pDeviceContext->OMSetDepthStencilState(mDepthStencilState, 1);

	pDeviceContext->RSSetViewports(1, &mViewport);
	pDeviceContext->RSSetScissorRects(1, &mRect);
	
}
void  ShadowMapClass::ClearRTV(ID3D11DeviceContext* pDeviceContext)
{
	pDeviceContext->ClearDepthStencilView(mDSV, D3D11_CLEAR_DEPTH, 0.0f, 0);
}
void  ShadowMapClass::UnbindRTV(ID3D11DeviceContext* pDeviceContext)
{
	//ID3D11RenderTargetView* nullRTV[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT] = { nullptr };
	//ID3D11DepthStencilView* nullDSV = nullptr;
	//ID3D11DepthStencilState* nullDSS = nullptr;

	//// Bind the render target view array and depth stencil buffer to the output render pipeline.
	//pDeviceContext->OMSetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, nullRTV, nullDSV);
	//pDeviceContext->OMSetDepthStencilState(nullDSS, 1);

	pDeviceContext->RSSetViewports(1, &prevVP);
	pDeviceContext->RSSetScissorRects(1, &mPR);

}

bool ShadowMapClass::InitShader(ID3D11Device* pDevice, WCHAR* vFile, WCHAR* pFile, WCHAR* gFile, float w, float h)
{
	bool result;
	HRESULT hr;
	mPR.bottom = (LONG)h;
	mPR.right = (LONG)w;
	mPR.left = 0;
	mPR.top = 0;
	prevVP.Height = h;
	prevVP.Width = w;
	prevVP.TopLeftX = 0;
	prevVP.TopLeftY = 0;
	prevVP.MaxDepth = 1.0f;
	prevVP.MinDepth = 0.0f;

	w = 1024;
	h = 1024;
	mRect.bottom = (LONG)h*SHADOW_DETAIL;
	mRect.right = (LONG)w*SHADOW_DETAIL;
	mRect.left = 0;
	mRect.top = 0;


	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// Get a count of the elements in the layout.
	int numElements = sizeof(vertexDesc) / sizeof(vertexDesc[0]);

	result = ShaderClass::InitShader(pDevice, vFile, pFile, NULL, vertexDesc, numElements);
	if (!result)
	{
		return false;
	}


	D3D11_TEXTURE2D_DESC depthBufferDesc;
	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = (UINT)w*SHADOW_DETAIL;
	depthBufferDesc.Height = (UINT)h*SHADOW_DETAIL;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_R32_TYPELESS;// DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

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

	
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;// DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	hr = pDevice->CreateDepthStencilView(mDSB, &depthStencilViewDesc, &mDSV);
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

	hr = pDevice->CreateShaderResourceView(mDSB, &shaderResourceViewDesc, &mSRV);
	if (FAILED(hr))
	{
		return false;
	}



	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;


	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;// ;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	if (mDepthStencilState)
	{
		mDepthStencilState->Release();
		mDepthStencilState = 0;
	}
	// Create the depth stencil state.
	hr = pDevice->CreateDepthStencilState(&depthStencilDesc, &mDepthStencilState);
	if (FAILED(hr))
	{
		MessageBox(0, L"Could not create mDepthStencilState.", 0, 0);
		return false;
	}



	// Setup the viewport for rendering.
	mViewport.Width = (float)w*SHADOW_DETAIL;
	mViewport.Height = (float)h * SHADOW_DETAIL;
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;
	mViewport.TopLeftX = 0.0f;
	mViewport.TopLeftY = 0.0f;

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