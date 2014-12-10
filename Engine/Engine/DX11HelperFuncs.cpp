#include "DX11HelperFuncs.h"

namespace DX11HelpF
{
	// Create Depth Stencil View and Stencil Buffer
	bool createDepthStencilViewAndBuffer(ID3D11Device* pDevice, D3D11_TEXTURE2D_DESC* pTDesc, ID3D11Texture2D** ppDepthStencilBuffer, ID3D11DepthStencilView** ppDepthStencilView)
	{
		HRESULT hr;

		hr = pDevice->CreateTexture2D(pTDesc, 0, ppDepthStencilBuffer);
		if (FAILED(hr))
		{
			MessageBox(0, L"Could not create DepthStencilBuffer.", 0, 0);
			return false;
		}

		hr = pDevice->CreateDepthStencilView(*ppDepthStencilBuffer, 0, ppDepthStencilView);
		if (FAILED(hr))
		{
			MessageBox(0, L"Could not create DepthStencilview.", 0, 0);
			return false;
		}

		return true;
	}

	// Create swapchain
	bool createSwapChain(IDXGIFactory* pFactory, ID3D11Device* pDevice, IDXGISwapChain** ppSwapChain, DXGI_SWAP_CHAIN_DESC* pSCD)
	{
		bool result;
		HRESULT hr;

		if (!pSCD)
			return false;

		if (!pDevice)
			return false;

		if (!pFactory)
		{
			return false;
		}

		hr = pFactory->CreateSwapChain(pDevice, pSCD, ppSwapChain);
		if (FAILED(hr))
		{
			MessageBox(0, L"CreateSwapChain Failed.", 0, 0);
			return false;
		}

		return true;
	}

	// Create Device and DeviceContext and/or Adapter
	bool createDeviceAndDeviceContext(D3D_DRIVER_TYPE pDriverType, UINT flags, IDXGIAdapter** ppAdapter, ID3D11Device** ppDevice, ID3D11DeviceContext** ppDeviceContext)
	{
		HRESULT hr;
		D3D_FEATURE_LEVEL featureLevel;

		if (ppAdapter)
		{
			// Create Device and Context
			hr = D3D11CreateDevice(
				*ppAdapter,
				pDriverType,
				NULL,
				flags,
				NULL,
				0,
				D3D11_SDK_VERSION,
				ppDevice,
				&featureLevel,
				ppDeviceContext);
		}
		else
		{
			// Create Device and Context
			hr = D3D11CreateDevice(
				NULL,
				pDriverType,
				NULL,
				flags,
				NULL,
				0,
				D3D11_SDK_VERSION,
				ppDevice,
				&featureLevel,
				ppDeviceContext);
		}

		if (FAILED(hr))
		{
			MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
			return false;
		}
		if (featureLevel != D3D_FEATURE_LEVEL_11_0)
		{
			MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
			return false;
		}

		return true;
	}

	// Create Device and DeviceContext and SwapChain and/or Adapter
	bool createDeviceAndDeviceContextAndSwapChain(D3D_DRIVER_TYPE pDriverType, UINT flags, IDXGIAdapter** ppAdapter, ID3D11Device** ppDevice, ID3D11DeviceContext** ppDeviceContext, IDXGISwapChain** ppSwapChain, DXGI_SWAP_CHAIN_DESC* pSCD)
	{
		HRESULT hr;
		D3D_FEATURE_LEVEL featureLevel;


		// Create Device and Context and SwapChain
		if (ppAdapter)
		{
			hr = D3D11CreateDeviceAndSwapChain(
				*ppAdapter,
				pDriverType,
				NULL,
				flags,
				NULL,
				0,
				D3D11_SDK_VERSION,
				pSCD,
				ppSwapChain,
				ppDevice,
				&featureLevel,
				ppDeviceContext);
		}
		else
		{
			hr = D3D11CreateDeviceAndSwapChain(
				NULL,
				pDriverType,
				NULL,
				flags,
				NULL,
				0,
				D3D11_SDK_VERSION,
				pSCD,
				ppSwapChain,
				ppDevice,
				&featureLevel,
				ppDeviceContext);
		}

		if (FAILED(hr))
		{
			MessageBox(0, L"D3D11CreateDevice and swapchain Failed.", 0, 0);
			return false;
		}
		if (featureLevel != D3D_FEATURE_LEVEL_11_0)
		{
			MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
			return false;
		}

		return true;
	}

	// Create DXGI Device
	bool createDxgiDevice(ID3D11Device* pDevice, IDXGIDevice** ppDxgiDevice)
	{
		HRESULT hr;
		if (!pDevice)
			return false;

		hr = pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)ppDxgiDevice);
		if (FAILED(hr))
		{
			MessageBox(0, L"QueryInterface Failed.", 0, 0);
			return false;
		}
		return true;
	}

	// Create DXGI Adapter
	bool createDxgiAdapter(IDXGIDevice* pDXGIDevice, IDXGIAdapter** ppDxgiAdapter)
	{
		bool result;
		HRESULT hr;
		if (!pDXGIDevice)
		{
			return false;
		}

		hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)ppDxgiAdapter);
		if (FAILED(hr))
		{
			MessageBox(0, L"Adapter GetParent Failed.", 0, 0);
			return false;
		}
		return true;
	}


	// Create DXGI Factory
	bool createDxgiFactory(IDXGIAdapter* pDXGIAdapter, IDXGIFactory** ppFactory)
	{
		bool result;
		HRESULT hr;
		if (!pDXGIAdapter)
		{
			return false;
		}

		hr = pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)ppFactory);
		if (FAILED(hr))
		{
			MessageBox(0, L"Factory GetParent Failed.", 0, 0);
			return false;
		}
		return true;
	}

	// Create Render Target View
	bool createRenderTargetView(ID3D11Device* pDevice, IDXGISwapChain* pSwapChain, D3D11_RENDER_TARGET_VIEW_DESC* pRTVDesc, ID3D11RenderTargetView** ppRenderTargetView)
	{
		HRESULT hr;

		// Get the backbuffer
		ID3D11Texture2D* backBuffer;
		hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
		if (FAILED(hr))
		{
			MessageBox(0, L"Failed to get backBuffer.", 0, 0);
			return false;
		}

		// Create render target view
		hr = pDevice->CreateRenderTargetView(backBuffer, pRTVDesc, ppRenderTargetView);
		if (FAILED(hr))
		{
			MessageBox(0, L"Failed to create render target view.", 0, 0);
			return false;
		}

		backBuffer->Release();

		return true;
	}

}

