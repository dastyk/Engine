#include "D3DApp.h"


D3DApp::D3DApp(HINSTANCE hInstance)
{
	mhAppInst = hInstance;
	mhMainWnd = nullptr;
	mAppPaused = false;
	mMinimized = false;
	mMaximized = false;
	mResizing = false;
	m4xMsaaQuality = 1;

	mSwapChain = nullptr;
	mDevice = nullptr;
	mDeviceContext = nullptr;
	mDepthStencilBuffer = nullptr;
	mRenderTargetView = nullptr;
	mDepthStencilView = nullptr;
	
	mInput = 0;

	mClientWidth = 32*COMPUTE_X;
	mClientHeight = 32*COMPUTE_Y;

	mNearPlane = 0.1f;
	mFarPlane = 10000;
	mFoV = XMConvertToRadians(90);

	mMainWndCaption = L"Engine";

	mDriverType = D3D_DRIVER_TYPE_HARDWARE;

	mEnable4xMsaa = false;

	mBlendingState = 0;
}

bool D3DApp::Init()
{
	bool result;

	result = InitMainWindow();
	if (!result)
	{
		MessageBox(0, L"Failed to Initiate main window", 0, 0);
		return false;
	}

	result = InitDirect3D();
	if (!result)
	{
		MessageBox(0, L"Failed to Initiate directx", 0, 0);
		return false;
	}

	mInput = new InputClass(mClientWidth, mClientHeight);
	if (!mInput)
	{
		MessageBox(0, L"Failed create input class", 0, 0);
		return false;
	}


	

	return true;
}


D3DApp::~D3DApp()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if (mSwapChain)
	{
		mSwapChain->SetFullscreenState(false, NULL);
	}

	if (mDepthStencilView)
	{
		mDepthStencilView->Release();
		mDepthStencilView = 0;
	}

	if (mDepthStencilBuffer)
	{
		mDepthStencilBuffer->Release();
		mDepthStencilBuffer = 0;
	}

	if (mRenderTargetView)
	{
		mRenderTargetView->Release();
		mRenderTargetView = 0;
	}

	if (mDeviceContext)
	{
		mDeviceContext->Release();
		mDeviceContext = 0;
	}

	if (mDevice)
	{
		mDevice->Release();
		mDevice = 0;
	}

	if (mSwapChain)
	{
		mSwapChain->Release();
		mSwapChain = 0;
	}
	if (mDxgiDevice)
	{
		mDxgiDevice->Release();
		mDxgiDevice = 0;
	}
	if (mDxgiAdapter)
	{
		mDxgiAdapter->Release();
		mDxgiAdapter = 0;
	}
	if (mDxgiFactory)
	{
		mDxgiFactory->Release();
		mDxgiFactory = 0;
	}
	if (mInput)
	{
		delete mInput;
		mInput = 0;
	}
	if (mBlendingState)
	{
		mBlendingState->Release();
		mBlendingState = 0;
	}
	
}


HINSTANCE D3DApp::AppInst()const
{
	return (HINSTANCE)mhAppInst;
}
HWND D3DApp::MainWnd()const
{
	return (HWND)mhMainWnd;
}

float D3DApp::AspectRatio()const
{
	return static_cast<float>(mClientWidth) / mClientHeight;
}

int D3DApp::Run()
{
	MSG msg = { 0 };
	mTimer.Reset();

	while (msg.message != WM_QUIT)
	{
		// If message to handle
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// game stuff
		else
		{
			mTimer.Tick();
			if (!mAppPaused)
			{
				CalculateFrameState();
				handleInput();
				UpdateScene(mTimer.DeltaTime());
				
				DrawScene();
			}
			else
			{
				Sleep(100);
			}
		}
	}
	return (int)msg.wParam;
}

bool D3DApp::InitMainWindow()
{
	WNDCLASSEX wc;
	int posX, posY;


	// Get an external pointer to this object.
	ApplicationHandle = this;


	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = mhAppInst;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = mMainWndCaption.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

		// Place the window in the middle of the screen.
	posX = (GetSystemMetrics(SM_CXSCREEN) - mClientWidth) / 2;
	posY = (GetSystemMetrics(SM_CYSCREEN) - mClientHeight) / 2;


	RECT rc = { 0, 0, mClientWidth, mClientHeight };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	// Create the window with the screen settings and get the handle to it.
	mhMainWnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		mMainWndCaption.c_str(),
		mMainWndCaption.c_str(),
		WS_OVERLAPPEDWINDOW,
		posX,
		posY,
		rc.right - rc.left,
		rc.bottom - rc.top,
		NULL,
		NULL,
		mhAppInst,
		NULL);

	if (!mhMainWnd)
	{
		MessageBox(0, L"Failed to create Window", 0, 0);
		return false;
	}

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(mhMainWnd, SW_SHOW);
	SetForegroundWindow(mhMainWnd);
	SetFocus(mhMainWnd);
	

	mScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	mScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	SetCursorPos(mScreenWidth/2, mScreenHeight/2);

	// Hide the mouse cursor.
	ShowCursor(true);

	return true;
}

LRESULT D3DApp::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			mAppPaused = true;
			mTimer.Stop();
		}
		else
		{
			mAppPaused = false;
			mTimer.Start();
		}
		return 0;
	case WM_ENTERSIZEMOVE:
		mAppPaused = true;
		mResizing = true;
		mTimer.Stop();
		return 0;
	case WM_EXITSIZEMOVE:
		mAppPaused = false;
		mResizing = false;
		mTimer.Start();
		OnResize();
		return 0;
	case WM_MENUCHAR:
		return MAKELRESULT(0, MNC_CLOSE);
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		return 0;

	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MOUSEMOVE:

		OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_SIZE: // If enter/exit fullscreen
		if (wParam == SIZE_MAXIMIZED)
			OnResize();
		return 0;
	case WM_KEYDOWN:
		mInput->keyDown((unsigned int)wParam);
		return 0;
	case WM_KEYUP:
		mInput->keyUp((unsigned int)wParam);
		return 0;

		// Any other messages send to the default message handler as our application won't make use of them.
	default:
	{
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	}

}



bool D3DApp::InitDirect3D()
{
	UINT createDeviceFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	bool result;

	// Create Device,DeviceContext,Swapchain and dxgiAdapter
	result = createDeviceAndDeviceContextAndSwapChain(mDriverType, createDeviceFlags);
	if (!result)
	{
		return false;
	}

	// Create render target view
	result = createRenderTargetView(NULL);
	if (!result)
	{
		return false;
	}

	// Create Depthstencilbuffer and view and set render targets
	result = createDepthStencilBufferView();
	if (!result)
	{
		return false;
	}

	mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	// Set viewport
	setViewPort(static_cast<float>(mClientWidth), static_cast<float>(mClientHeight));

	

	return true;
}

void D3DApp::CalculateFrameState()
{
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	if ((mTimer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt;
		float mspf = 1000.0f / fps;

		std::wostringstream outs;
		outs.precision(6);
		outs << mMainWndCaption << L"Fps: " << fps << L" Frame Time: " << mspf << L" ms";
		SetWindowText(mhMainWnd, outs.str().c_str());


		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}

bool D3DApp::createDepthStencilBufferView()
{
	HRESULT hr;



	// Create depth/stencil buffer and view
	D3D11_TEXTURE2D_DESC depthBufferDesc;

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = mClientWidth;
	depthBufferDesc.Height = mClientHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D32_FLOAT;
	if (mEnable4xMsaa)
	{
		depthBufferDesc.SampleDesc.Count = 4;
		depthBufferDesc.SampleDesc.Quality = m4xMsaaQuality - 1;

	}
	else
	{
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
	}
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;


	hr = mDevice->CreateTexture2D(&depthBufferDesc, 0, &mDepthStencilBuffer);
	if (FAILED(hr))
	{
		MessageBox(0, L"Could not create DepthStencilBuffer.", 0, 0);
		return false;
	}



	hr = mDevice->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView);
	if (FAILED(hr))
	{
		MessageBox(0, L"Could not create DepthStencilview.", 0, 0);
		return false;
	}


	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;


	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	
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

	// Create the depth stencil state.
	hr = mDevice->CreateDepthStencilState(&depthStencilDesc, &mDepthStencilState);
	if (FAILED(hr))
	{
		MessageBox(0, L"Could not create mDepthStencilState.", 0, 0);
		return false;
	}

	// Set the depth stencil state.
	mDeviceContext->OMSetDepthStencilState(mDepthStencilState, 1);
	

	D3D11_BLEND_DESC blendStateDescription;
	// Clear the blend state description.
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	// Create an alpha enabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = false;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create the blend state using the description.
	hr = mDevice->CreateBlendState(&blendStateDescription, &mBlendingState);
	if (FAILED(hr))
	{
		return false;
	}
	float color[4] = { 0.0f };
	mDeviceContext->OMSetBlendState(mBlendingState, color, 0xffffffff);

	/*
	// Create depth/stencil buffer and view
	D3D11_TEXTURE2D_DESC depthBufferDesc;

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = mClientWidth;
	depthBufferDesc.Height = mClientHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	if (mEnable4xMsaa)
	{
		depthBufferDesc.SampleDesc.Count = 4;
		depthBufferDesc.SampleDesc.Quality = m4xMsaaQuality - 1;

	}
	else
	{
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
	}
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	hr = mDevice->CreateTexture2D(&depthBufferDesc, 0, &mDepthStencilBuffer);
	if (FAILED(hr))
	{
		MessageBox(0, L"Could not create DepthStencilBuffer.", 0, 0);
		return false;
	}

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

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

	// Create the depth stencil state.
	hr = mDevice->CreateDepthStencilState(&depthStencilDesc, &mDepthStencilState);
	if (FAILED(hr))
	{
		MessageBox(0, L"Could not create mDepthStencilState.", 0, 0);
		return false;
	}

	// Set the depth stencil state.
	mDeviceContext->OMSetDepthStencilState(mDepthStencilState, 1);

	
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	// Initailze the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	hr = mDevice->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView);
	if (FAILED(hr))
	{
		MessageBox(0, L"Could not create DepthStencilview.", 0, 0);
		return false;
	}
	
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	*/
	D3D11_RASTERIZER_DESC rasterDesc;
	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = false;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	hr = mDevice->CreateRasterizerState(&rasterDesc, &mRasterState);
	if (FAILED(hr))
	{
		return false;
	}

	// Now set the rasterizer state.
	mDeviceContext->RSSetState(mRasterState);
	

	return true;
}

void D3DApp::OnResize()
{
	bool result;
	RECT rc;
	


	GetWindowRect(mhMainWnd, &rc);
	mClientWidth = rc.right - rc.left-16;
	mClientHeight = rc.bottom - rc.top-38;

	if (mSwapChain)
	{
		mSwapChain->Release();
		mSwapChain = 0;
	}
	

	result = createSwapChain();
	if (!result)
	{
		return;
	}

	result = createRenderTargetView(NULL);
	if (!result)
	{
		return;
	}

	mDepthStencilBuffer->Release();
	mDepthStencilView->Release();

	createDepthStencilBufferView();

	

	// Set viewport
	setViewPort(static_cast<float>(mClientWidth), static_cast<float>(mClientHeight));
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

		// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

		// All other messages pass to the message handler in the system class.
	default:
	{
		return ApplicationHandle->MsgProc(hwnd, umessage, wparam, lparam);
	}
	}
}

bool D3DApp::createSwapChain()
{
	bool result;
	HRESULT hr;
	


	if (!mDxgiFactory)
	{
		result = createDxgiFactory();
		if (!result)
			return false;
	}

	if (mSwapChain)
	{
		mSwapChain->Release();
		mSwapChain = 0;
	}

	DXGI_SWAP_CHAIN_DESC sd;
	// clear out the struct for use
	ZeroMemory(&sd, sizeof(sd));

	sd.BufferDesc.Width = mClientWidth;
	sd.BufferDesc.Height = mClientHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	if (mEnable4xMsaa)
	{
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = m4xMsaaQuality - 1;

	}
	else
	{
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = mhMainWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;



	hr = mDxgiFactory->CreateSwapChain(mDevice, &sd, &mSwapChain);
	if (FAILED(hr))
	{
		MessageBox(0, L"CreateSwapChain Failed.", 0, 0);
		return false;
	}

	return true;
}

bool D3DApp::createDeviceAndDeviceContext( D3D_DRIVER_TYPE pDriverType, UINT flags)
{
	HRESULT hr;
	D3D_FEATURE_LEVEL featureLevel;

	// Create Device and Context
	hr = D3D11CreateDevice(
		mDxgiAdapter,
		pDriverType,
		NULL,
		flags,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&mDevice,
		&featureLevel,
		&mDeviceContext);

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

bool D3DApp::createDeviceAndDeviceContextAndSwapChain(D3D_DRIVER_TYPE pDriverType, UINT flags)
{
	HRESULT hr;
	D3D_FEATURE_LEVEL featureLevel;

	DXGI_SWAP_CHAIN_DESC sd;
	// clear out the struct for use
	ZeroMemory(&sd, sizeof(sd));

	sd.BufferDesc.Width = mClientWidth;
	sd.BufferDesc.Height = mClientHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	if (mEnable4xMsaa)
	{
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = m4xMsaaQuality - 1;

	}
	else
	{
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_UNORDERED_ACCESS;
	sd.BufferCount = 1;
	sd.OutputWindow = mhMainWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// Create Device and Context and SwapChain
	hr = D3D11CreateDeviceAndSwapChain(
		mDxgiAdapter,
		pDriverType,
		NULL,
		flags,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&mSwapChain,
		&mDevice,
		&featureLevel,
		&mDeviceContext);
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

bool D3DApp::createDxgiDevice()
{
	HRESULT hr;
	if (!mDevice)
		return false;
	if (mDxgiDevice)
	{
		mDxgiDevice->Release();
		mDxgiDevice = 0;
	}
	hr = mDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&mDxgiDevice);
	if (FAILED(hr))
	{
		MessageBox(0, L"QueryInterface Failed.", 0, 0);
		return false;
	}
	return true;
}

bool D3DApp::createDxgiAdapter()
{
	bool result;
	HRESULT hr;
	if (!mDxgiDevice)
	{
		result = createDxgiDevice();
		if (!result)
			return false;
	}
	if (mDxgiAdapter)
	{
		mDxgiAdapter->Release();
		mDxgiAdapter = 0;
	}
	hr = mDxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&mDxgiAdapter);
	if (FAILED(hr))
	{
		MessageBox(0, L"Adapter GetParent Failed.", 0, 0);
		return false;
	}
	return true;
}

bool D3DApp::createDxgiFactory()
{
	bool result;
	HRESULT hr;
	if (!mDxgiAdapter)
	{
		result = createDxgiAdapter();
		if (!result)
			return false;
	}
	if (mDxgiFactory)
	{
		mDxgiFactory->Release();
		mDxgiFactory = 0;
	}
	hr = mDxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&mDxgiFactory);
	if (FAILED(hr))
	{
		MessageBox(0, L"Factory GetParent Failed.", 0, 0);
		return false;
	}
	return true;
}

bool D3DApp::createRenderTargetView(D3D11_RENDER_TARGET_VIEW_DESC* pRTVDesc)
{
	HRESULT hr;

	// Create render target view
	ID3D11Texture2D* backBuffer;
	hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	if (FAILED(hr))
	{
		return false;
	}
#if defined(DEBUG) || defined(_DEBUG)
	if (mDebugger)
	{
		mDebugger->Release();
		mDebugger = 0;
	}
	hr = mDevice->QueryInterface(__uuidof(ID3D11Debug), (void**)&mDebugger);
	if (FAILED(hr))
	{
		return false;
	}
#endif
	if (mRenderTargetView)
	{
		mRenderTargetView->Release();
		mRenderTargetView = 0;
	}
	hr = mDevice->CreateRenderTargetView(backBuffer, pRTVDesc, &mRenderTargetView);
	if (FAILED(hr))
	{
		return false;
	}
	backBuffer->Release();

	return true;
}

void D3DApp::setViewPort(float width, float height)
{
	// Set viewport
	mScreenViewport.Width = width;
	mScreenViewport.Height = height;
	mScreenViewport.MinDepth = 0.0f;
	mScreenViewport.MaxDepth = 1.0f;
	mScreenViewport.TopLeftX = 0;
	mScreenViewport.TopLeftY = 0;

	mDeviceContext->RSSetViewports(1, &mScreenViewport);
}

