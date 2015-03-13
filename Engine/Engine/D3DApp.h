#ifndef _D3DAPP_H_
#define _D3DAPP_H_

#pragma once

#include <windows.h>
#include <windowsx.h>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <string>
#include <sstream>
#include "GameTimer.h"
#include "InputClass.h"


using namespace DirectX;

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#define COMPUTE_X 25//58
#define COMPUTE_Y 20//28

#define TIME 0


class D3DApp
{
public:
	D3DApp(HINSTANCE hInstance);
	virtual ~D3DApp();

	HINSTANCE AppInst()const;
	HWND MainWnd()const;
	float AspectRatio()const;

	int Run();

	virtual bool Init();
	virtual void OnResize();

	virtual void handleInput() = 0;
	virtual void UpdateScene(float dt) = 0;
	virtual void DrawScene() = 0;
	virtual LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	virtual void OnMouseDown(WPARAM btnState, int x, int y){}
	virtual void OnMouseUp(WPARAM btnState, int x, int y){}
	virtual void OnMouseMove(WPARAM btnState, int x, int y){}
	

protected:
	bool InitMainWindow();
	bool InitDirect3D();
	void CalculateFrameState();
	bool createDepthStencilBufferView();
	bool createSwapChain();
	bool createDeviceAndDeviceContext(D3D_DRIVER_TYPE pDriverType, UINT flags);
	bool createDeviceAndDeviceContextAndSwapChain(D3D_DRIVER_TYPE pDriverType, UINT flags);
	bool createDxgiDevice();
	bool createDxgiAdapter();
	bool createDxgiFactory();
	bool createRenderTargetView(D3D11_RENDER_TARGET_VIEW_DESC* pRTVDesc);

	void setViewPort(float width,float height);



protected:
	

protected:
	IDXGIDevice* mDxgiDevice;
	IDXGIAdapter* mDxgiAdapter;
	IDXGIFactory* mDxgiFactory;
protected:
	HINSTANCE mhAppInst;
	HWND mhMainWnd;
	bool mAppPaused;
	bool mMinimized;
	bool mMaximized;
	bool mResizing;
	UINT m4xMsaaQuality; // Quality level of 4x MSAA

	GameTimer mTimer;
	



	IDXGISwapChain* mSwapChain;
	ID3D11Device* mDevice;
	ID3D11DeviceContext* mDeviceContext;
	ID3D11Texture2D* mDepthStencilBuffer;	
	ID3D11RenderTargetView* mRenderTargetView;
	ID3D11DepthStencilView* mDepthStencilView;
	ID3D11DepthStencilState* mDepthStencilState;
	ID3D11RasterizerState* mRasterState;
	ID3D11BlendState* mBlendingState;

	D3D11_VIEWPORT mScreenViewport;


	std::wstring mMainWndCaption;

	D3D_DRIVER_TYPE mDriverType;





protected:

	InputClass* mInput;



	int mClientWidth;
	int mClientHeight;

	int mScreenWidth;
	int mScreenHeight;

	bool mEnable4xMsaa; 

	float mNearPlane, mFarPlane, mFoV;

#if defined(DEBUG) || defined(_DEBUG)
	ID3D11Debug* mDebugger;
#endif
};
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static D3DApp* ApplicationHandle = 0;
#endif