#include "InitDirect3DApp.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	InitDirect3DApp app(hInstance);
	if (!app.Init())
		return 0;

	return app.Run();


}



InitDirect3DApp::InitDirect3DApp(HINSTANCE hInstance) : D3DApp(hInstance)
{
	mColorShader = 0;
	mTexShader = 0;
	mLightShader = 0;
	mTerrainShader = 0;

	mModel = 0;
	mObject = 0;

	mSun = 0;

	mTerrain = 0;
	mTerrainModel = 0;

	mDrawDistFog = 0;

	mCamera = 0;

	

	mEnable4xMsaa = true;
	m4xMsaaQuality = 1;
}


InitDirect3DApp::~InitDirect3DApp()
{
	if (mCamera)
	{
		delete mCamera;
		mCamera = 0;
	}
	if (mLightShader)
	{
		delete mLightShader;
		mLightShader = 0;
	}
	if (mTexShader)
	{
		delete mTexShader;
		mTexShader = 0;
	}
	if (mColorShader)
	{
		delete mColorShader;
		mColorShader = 0;
	}
	if (mModel)
	{
		delete mModel;
		mModel = 0;
	}
	if (mObject)
	{
		delete mObject;
		mObject = 0;
	}
	if (mDrawDistFog)
	{
		delete mDrawDistFog;
		mDrawDistFog = 0;
	}
	if (mSun)
	{
		delete mSun;
		mSun = 0;
	}
	if (mTerrain)
	{
		delete mTerrain;
		mTerrain = 0;
	}
	if (mTerrainModel)
	{
		delete mTerrainModel;
		mTerrainModel = 0;
	}
	if (mTerrainShader)
	{
		delete mTerrainShader;
		mTerrainShader = 0;
	}
}

bool InitDirect3DApp::Init()
{
	bool result;

	if (!D3DApp::Init())
		return false;

	mCamera = new CameraClass();
	if (!mCamera)
		return false;

	mCamera->SetProjMatrix(mFoV, AspectRatio(), mNearPlane, mFarPlane);
	XMFLOAT3 pos(0.0, 0.0, -10.0);
	mCamera->SetPosition(pos);

	mTexShader = new TextureShaderClass();
	if (!mTexShader)
		return false;

	result = mTexShader->Init(mDevice);
	if (!result)
		return false;

	mColorShader = new ColorShaderClass();
	if (!mColorShader)
		return false;

	result = mColorShader->Init(mDevice);
	if (!result)
		return false;
	
	mLightShader = new LightShaderClass();
	if (!mLightShader)
		return false;

	result = mLightShader->Init(mDevice);
	if (!result)
		return false;

	mModel = new ModelClass();
	if (!mModel)
		return false;

	result = mModel->createModel(mDevice,L"data/resources/BTH_ny.jpg");
	if (!result)
		return false;

	mObject = new ObjectClass(mModel);
	if (!mObject)
		return false;

	mDrawDistFog = new FogClass(XMFLOAT3(0, 0, 0), XMFLOAT3(0.4, 0.4, 0.9f), 50);
	if (!mDrawDistFog)
		return false;

	mSun = new LightObjectClass();
	if (!mSun)
		return false;

	LightClass* light = mSun->GetAmbientLight();
	light->SetLightColor(XMFLOAT3(0.5, 0.5, 0.5));
	light = mSun->GetDiffuseLight();
	light->SetLightColor(XMFLOAT3(0.7, 0.7, 0.7));


	mTerrainModel = new TerrainClass();
	if (!mTerrainModel)
		return false;

	result = mTerrainModel->Init(mDevice);
	if (!result)
		return false;

	mTerrain = new ObjectClass(mTerrainModel);
	if (!mTerrain)
		return false;

	mTerrainShader = new TerrainShaderClass();
	if (!mTerrainShader)
		return false;

	result = mTerrainShader->Init(mDevice);
	if (!result)
		return false;

	return true;
}

void InitDirect3DApp::OnResize()
{
	D3DApp::OnResize();

	if (mCamera)
		mCamera->SetProjMatrix(mFoV, AspectRatio(), mNearPlane, mFarPlane);
}

void InitDirect3DApp::UpdateScene(float dt)
{
	TransformationClass* temp = mObject->GetTransformation();
	XMFLOAT3 rot = temp->GetRotation();
	XMFLOAT3 pos = temp->GetPosition();
	//pos.y = 2;
	rot.y -= dt * 25;
	//rot.x += dt * 25;
	//rot.z += dt * 25;
	temp->SetRotation(rot);
	temp->SetPosition(pos);

	mCamera->CalViewMatrix();
}

void InitDirect3DApp::DrawScene()
{
	HRESULT hr;
	bool result;

	assert(mDeviceContext);
	assert(mSwapChain);




	// Clear back buffer blue.
	float clearColor[] = { 0.4, 0.4, 0.9f, 1 };
	mDeviceContext->ClearRenderTargetView(mRenderTargetView, clearColor);
	

	// Clear depth buffer to 1.0f and stencil buffer to 0.
	mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);


	//mTerrain->SetAsModelToBeDrawn(mDeviceContext);

	//XMFLOAT4X4 I;
	//XMStoreFloat4x4(&I, XMMatrixIdentity());


	mTerrain->SetAsObjectToBeDrawn(mDeviceContext);

	result = mTerrainShader->Render(
		mDeviceContext,
		mTerrain,
		mCamera,
		mSun,
		mObject->GetMaterial(),
		mDrawDistFog);

	mObject->SetAsObjectToBeDrawn(mDeviceContext);

	result = mLightShader->Render(
		mDeviceContext,
		mObject,
		mCamera,
		mSun,
		mObject->GetMaterial(),
		mDrawDistFog);

	if (!result)
	{
		MessageBox(0, L"Failed to Render Shaders", 0, 0);
		return;
	}


	/*result = mTexShader->Render(mDeviceContext, mObject->GetIndexCount(), mObject->GetWorldMatrix(), mCamera.GetViewMatrix(), mCamera.GetProjMatrix(),mObject->GetShaderResourceView());
	if (!result)
	{
		MessageBox(0, L"Failed to Render Shaders", 0, 0);
		return;
	}*/
	
	// Present the back buffer to the screen
	hr = mSwapChain->Present(0, 0);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to swap back buffer", 0, 0);
		return;
	}
}

void InitDirect3DApp::OnMouseDown(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{

		mCamera->moveForward();
		
	}
	else if ((btnState & MK_RBUTTON) != 0)
	{
		XMFLOAT3 pos = mCamera->GetPosition();
		pos.z -= 1;
		mCamera->SetPosition(pos);

	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;

	SetCapture(mhMainWnd);
}

void InitDirect3DApp::OnMouseUp(WPARAM btnState, int x, int y)
{

	ReleaseCapture();
}

void InitDirect3DApp::OnMouseMove(WPARAM btnState, int x, int y)
{
	
	if (btnState)
	{
		float dx = (float)(x - mLastMousePos.x);
		float dy = (float)(y - mLastMousePos.y);

		XMFLOAT3 rot = mCamera->GetRotation();
		rot.x += dx / 10;
		rot.y += dy / 10;
		mCamera->SetRotation(rot);
	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;


}
