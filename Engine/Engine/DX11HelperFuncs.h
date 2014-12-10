#ifndef _DX11HELPERFUNCS_H_
#define _DX11HELPERFUNCS_H_
#pragma once
#include <d3d11.h>

#pragma comment (lib, "d3d11.lib")
namespace DX11HelpF
{
	///////////////////////////////////
	// Main DirectX create functions //
	///////////////////////////////////

	// Create Depth Stencil View and Stencil Buffer
	bool createDepthStencilViewAndBuffer(ID3D11Device* pDevice, D3D11_TEXTURE2D_DESC* pTDesc, ID3D11Texture2D** ppDepthStencilBuffer, ID3D11DepthStencilView** ppDepthStencilView);

	// Create swapchain.
	bool createSwapChain(IDXGIFactory* pFactory, ID3D11Device* pDevice, IDXGISwapChain** ppSwapChain, DXGI_SWAP_CHAIN_DESC* pSCD);

	// Create Device and DeviceContext and/or Adapter
	bool createDeviceAndDeviceContext(D3D_DRIVER_TYPE pDriverType, UINT flags, IDXGIAdapter** ppAdapter, ID3D11Device** ppDevice, ID3D11DeviceContext** ppDeviceContext);

	// Create Device and DeviceContext and SwapChain and/or Adapter
	bool createDeviceAndDeviceContextAndSwapChain(D3D_DRIVER_TYPE pDriverType, UINT flags, IDXGIAdapter** ppAdapter, ID3D11Device** ppDevice, ID3D11DeviceContext** ppDeviceContext, IDXGISwapChain** ppSwapChain, DXGI_SWAP_CHAIN_DESC* pSCD);

	// Create DXGI Device
	bool createDxgiDevice(ID3D11Device* pDevice, IDXGIDevice** ppDxgiDevice);

	// Create DXGI Adapter
	bool createDxgiAdapter(IDXGIDevice* pDXGIDevice, IDXGIAdapter** ppDxgiAdapter);

	// Create DXGI Factory
	bool createDxgiFactory(IDXGIAdapter* pDXGIAdapter, IDXGIFactory** ppFactory);

	// Create Render Target View
	bool createRenderTargetView(ID3D11Device* pDevice, IDXGISwapChain* pSwapChain, D3D11_RENDER_TARGET_VIEW_DESC* pRTVDesc, ID3D11RenderTargetView** ppRenderTargetView);

}



#endif