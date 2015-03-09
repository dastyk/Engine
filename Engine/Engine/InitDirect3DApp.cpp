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
	mParticleShader = 0;
	mFirework = 0;

	mModel = 0;
	mQuadModel = 0;
	for (int i = 0; i < BUFFER_COUNT; i++)
	{
		mRTQ[i] = 0;
	}


	mObject = 0;
	mNRofObjects = 0;


	mSun = 0;

	mTerrain = 0;
	mTerrainModel = 0;
	mQuadTree = 0;


	mDrawDistFog = 0;

	mCamera = 0;


	mDeferredBuffer = nullptr;
	mDeferredShader = nullptr;

	mEnable4xMsaa = false;
	m4xMsaaQuality = 1;


	mPointLight = 0;
	mLightCount = 0;

	srand(time(NULL));

	mShadowmapShader = 0;
	mBoundingBoxShader = 0;
	mDCShader = 0;
	

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
		for (UINT i = 0; i < mNRofObjects; i++)
		{ 
			if (mObject[i])
			{
				delete mObject[i];
				mObject[i] = 0;
			}
		}
		delete[] mObject;
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

	if (mQuadTree)
	{
		delete mQuadTree;
		mQuadTree = 0;
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
	if (mParticleShader)
	{
		delete mParticleShader;
		mParticleShader = 0;
	}
	if (mFirework)
	{
		delete mFirework;
		mFirework = 0;
	}
	if (mDeferredBuffer)
	{
		delete mDeferredBuffer;
		mDeferredBuffer = 0;
	}
	if (mDeferredShader)
	{
		delete mDeferredShader;
		mDeferredShader = 0;
	}
	if (mPointLight)
	{
		for (UINT i = 0; i < mLightCount; i++)
		{
			if (mPointLight[i])
			{
				delete mPointLight[i];
				mPointLight[i] = 0;
			}
		}

		delete[]mPointLight;
		mPointLight = 0;

	}

	for (int i = 0; i < BUFFER_COUNT; i++)
	{
		delete mRTQ[i];
		mRTQ[i] = 0;
	}


	if (mQuadModel)
	{
		delete mQuadModel;
		mQuadModel = 0;
	}
	if (mShadowmapShader)
	{
		delete mShadowmapShader;
		mShadowmapShader = 0;
	}
	if (mBoundingBoxShader)
	{
		delete mBoundingBoxShader;
		mBoundingBoxShader = 0;
	}
	if (mDCShader)
	{
		delete mDCShader;
		mDCShader = 0;
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
	XMFLOAT3  pos(128, 0, 128);// pos(0.0f, 0.0f, 0.0f);
	mCamera->SetPosition(pos);
	mCamera->SetMoveSpeed(50);

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

	result = mModel->createModel(mDevice,"data/resources/soldier.smf");
	if (!result)
		return false;

	mNRofObjects = 1;
	mObject = new ObjectClass*[mNRofObjects];
	if (!mObject)
		return false;

	for (UINT i = 0; i < mNRofObjects; i++)
	{
		mObject[i] = new ObjectClass(mModel);
		TransformationClass* t = mObject[i]->GetTransformation();
		t->SetPosition(XMFLOAT3(rand() % 256, 0, rand() % 256));
		t->SetScale(XMFLOAT3(0.1, 0.1, 0.1));
	}
	

	mDrawDistFog = new FogClass(XMFLOAT3(0, 0, 0), XMFLOAT3(0.4f, 0.4f, 0.9f), 10);
	if (!mDrawDistFog)
		return false;

	mSun = new LightObjectClass();
	if (!mSun)
		return false;

	XMFLOAT3 dir = XMFLOAT3(-1, 0, 0);
	mSun->GetDiffuseLight()->SetLightDir(dir);

	mTerrainModel = new TerrainClass();
	if (!mTerrainModel)
		return false;

	result = mTerrainModel->Init(mDevice, &mQuadTree);
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

	



	mParticleShader = new ParticleShaderClass;
	if (!mParticleShader)
		return false;

	result = mParticleShader->Init(mDevice);
	if (!result)
		return false;

	mFirework = new FireworkEffect();
	if (!mFirework)
		return false;

	result = mFirework->Init(mDevice);
	if (!result)
		return false;

	
	mDeferredBuffer = new DeferredBufferClass();
	if (!mDeferredBuffer)
		return false;

	result = mDeferredBuffer->Init(mDevice, mClientWidth, mClientHeight);
	if (!result)
	{
		MessageBox(0, L"Failed init defferedBuffer", 0, 0);
		return false;
	}

	mDeferredShader = new DeferredShaderClass();
	if (!mDeferredShader)
		return false;

	result = mDeferredShader->Init(mDevice);
	if (!result)
	{
		MessageBox(0, L"Failed init defferedshader", 0, 0);
		return false;
	}

	//mLightCount = 10;
	//mPointLight = new PointLightClass*[mLightCount];
	//if (!mPointLight)
	//	return false;

	//for (int i = 0; i < mLightCount; i++)
	//{
	//	float l = 1; rand() % 10 / 10.0f;

	//	mPointLight[i] = new PointLightClass(XMFLOAT3(l, l, l), XMFLOAT3(rand() % 50 - 25, rand() % 100, rand() % 50 - 25), rand() % 100 + 20);
	//}

	mLightCount = 20;
	mPointLight = new PointLightClass*[mLightCount]; 
	if (!mPointLight)
		return false;


	mPointLight[1] = new PointLightClass(XMFLOAT3(0.5, 0.5, 0.5), XMFLOAT3(0, 0, 0), 50);
	mPointLight[0] = new PointLightClass(XMFLOAT3(0.025, 0.025, 0.025), XMFLOAT3(128, 275, 315), 10);
	mPointLight[0]->SetLightDir(XMFLOAT3(0, -1, -1));
	mPointLight[0]->SetProjMatrix(mFoV, AspectRatio(), mNearPlane, mFarPlane);

	for (UINT i = 2; i < mLightCount; i++)
	{
		float l = 1;rand() % 10 / 10.0f;
		XMFLOAT3 lp = XMFLOAT3(rand() % 256, 0, rand() % 256);
		lp.y = mTerrainModel->getHeightAtPoint(lp) + 5;
		mPointLight[i] = new PointLightClass(XMFLOAT3(rand() % 10 / 10.0f, rand() % 10 / 10.0f, rand() % 10 / 10.0f), lp, 10);
	}


	mQuadModel = new ModelClass();
	if (!mQuadModel)
		return false;
	result = mQuadModel->createModel(mDevice, "data/resources/quad.smf");
	if (!result)
	{
		return false;
	}

	for (int i = 0; i < BUFFER_COUNT; i++)
	{
		mRTQ[i] = new ObjectClass(mQuadModel);
		TransformationClass* t = mRTQ[i]->GetTransformation();	
		t->SetPosition(XMFLOAT3(-0.666666 + 0.666666*i, -0.666666f, 0));
	}

	mShadowmapShader = new ShadowMapClass();
	if (!mDeferredShader)
		return false;

	result = mShadowmapShader->Init(mDevice, mClientWidth, mClientHeight);
	if (!result)
	{
		MessageBox(0, L"Failed init shadowmap shader", 0, 0);
		return false;
	}
	
	mBoundingBoxShader = new BoundingBoxShader();
	if (!mBoundingBoxShader)
		return false;

	result = mBoundingBoxShader->Init(mDevice);
	if (!result)
	{
		MessageBox(0, L"Failed init boundingbox shader", 0, 0);
		return false;
	}
	

	
	TransformationClass* temp;
	for (UINT i = 0; i < mNRofObjects; i++)
	{
		temp = mObject[i]->GetTransformation();
		pos = temp->GetPosition();
		pos.y = mTerrainModel->getHeightAtPoint(pos) + 0.5;
		temp->SetPosition(pos);
	}

	mQuadTree->AddModels(mObject, mNRofObjects);
	mQuadTree->AddLights(mPointLight, mLightCount);
	mQuadTree->AddSnow(mDevice);

	mDCShader = new DeferredComputeShaderClass;
	if (!mDCShader)
		return false;

	result = mDCShader->Init(mDevice, mSwapChain);
	if (!result)
		return false;

	pos = mCamera->GetPosition();
	pos.y = mTerrainModel->getHeightAtPoint(pos) + 4.0f;
	mCamera->SetPosition(pos);

	for (UINT i = 0; i < GPU_TIMER_COUNT; i++)
	{
		mGPUTimer[i].Init(mDevice);
	}


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

	mCamera->SetUpdateTime(dt);

	TransformationClass* temp = 0;
	XMFLOAT3 rot;
	XMFLOAT3 pos;
	for (UINT i = 0; i < mNRofObjects; i++)
	{
		temp = mObject[i]->GetTransformation();
		rot = temp->GetRotation();
		pos = temp->GetPosition();
		rot.y += dt*100;
		pos.y = mTerrainModel->getHeightAtPoint(pos)+ 0.15;


		temp->SetRotation(rot);
		temp->SetPosition(pos);

		mObject[i]->Update();
	}

	//pos.y = 2;
	//rot.y -= dt * 25;
	//rot.x = 90;
	//rot.z += dt * 25;

	
	pos = mCamera->GetPosition();

	mPointLight[1]->SetLightPos(pos);
	rot = mPointLight[0]->GetLightDir();
	//rot.y += dt/10;
	mPointLight[0]->SetLightDir(rot);
	mPointLight[0]->CalcViewMatrix();


	pos.y = mTerrainModel->getHeightAtPoint(pos) + 4.0f;

	if (!mInput->isKeyDown(VK_F))
		mCamera->SetPosition(pos);

	

	/*std::wostringstream outs;
	outs.precision(6);
	outs << mMainWndCaption << mFirework->GetAliveParticles() + mSnow->GetAliveParticles();
	SetWindowText(mhMainWnd, outs.str().c_str());*/

	

	
	//mFirework->Update(dt);

	mCamera->CalcViewMatrix();

	mQuadTree->Update(dt, mCamera);
	
}

void InitDirect3DApp::handleInput()
{
	mCamera->MoveForward(mInput->isKeyDown(VK_W));
	mCamera->MoveBackward(mInput->isKeyDown(VK_S));
	mCamera->MoveRight(mInput->isKeyDown(VK_D));
	mCamera->MoveLeft(mInput->isKeyDown(VK_A));
	mCamera->MoveUpward(mInput->isKeyDown(VK_SPACE));
	mCamera->MoveDownward(mInput->isKeyDown(VK_SHIFT));
	if (mInput->isKeyDown(VK_ESCAPE))
	{
		PostQuitMessage(0);
	}
	mCamera->SetDC(mInput->isKeyDown(VK_C));
}

void InitDirect3DApp::DrawScene()
{
	HRESULT hr;
	bool result;
	int counter = 0;
	assert(mDeviceContext);
	assert(mSwapChain);

	BoundingFrustum f = mCamera->GetBoundingFrustum();
	BoundingFrustum f2 = mPointLight[0]->GetBoundingFrustum();
	UINT count = 0;

	mGPUTimer[0].TimeStart(mDeviceContext);

	mShadowmapShader->SetRTV(mDeviceContext);
	mShadowmapShader->ClearRTV(mDeviceContext);

	for (UINT i = 0; i < mNRofObjects; i++)
	{
		mObject[i]->SetAsObjectToBeDrawn(mDeviceContext, 0);
		result = mShadowmapShader->CreateShadowMap(mDeviceContext, mObject[i], mPointLight[0]);
		if (!result)
		{
			MessageBox(0, L"Failed to Render Shaders", 0, 0);
			return;
		}

	}

	mShadowmapShader->UnbindRTV(mDeviceContext);

	mGPUTimer[0].TimeEnd(mDeviceContext);


	mGPUTimer[1].TimeStart(mDeviceContext);

	mDeferredBuffer->SetRenderTargets(mDeviceContext);
	mDeferredBuffer->ClearRenderTargets(mDeviceContext, 0.0f, 0.0f, 0.0f, 0.0f);


	counter = mQuadTree->RenderAgainsQuadTree(
		mDeviceContext,
		mTerrainShader,
		mDeferredShader,
		mTerrain,
		mCamera,
		mPointLight[0],
		mShadowmapShader->GetShaderResourceView());

	


	



	mDeferredBuffer->UnsetRenderTargets(mDeviceContext);

	mGPUTimer[1].TimeEnd(mDeviceContext);
	
	mGPUTimer[2].TimeStart(mDeviceContext);

	mDeferredBuffer->SetLightRT(mDeviceContext);
	//mDeferredShader->SetLP(mDeviceContext, mCamera, mDeferredBuffer);
	result = mDeferredShader->RenderLights(mDeviceContext, mCamera, mDeferredBuffer, mPointLight, mLightCount);
	if (!result)
	{
		MessageBox(0, L"Failed to Render Shaders", 0, 0);
		return;
	}
	//mQuadTree->RenderLightsAgainsQuadTree(mDeviceContext, mDeferredBuffer, mDeferredShader, mCamera);

	//mDeferredShader->UnSetLP(mDeviceContext);
	mDeferredBuffer->UnsetRenderTargets(mDeviceContext);

	mGPUTimer[2].TimeEnd(mDeviceContext);



	//// Clear back buffer blue.
	float clearColor[] = { 0.4f, 0.4f, 0.9f, 1.0f };
	mDeviceContext->ClearRenderTargetView(mRenderTargetView, clearColor);

	mGPUTimer[3].TimeStart(mDeviceContext);
	mDeferredShader->Render(mDeviceContext, mDeferredBuffer);
	//mDCShader->Compute(mDeviceContext, mDeferredBuffer, COMPUTE_X, COMPUTE_Y);
	mGPUTimer[3].TimeEnd(mDeviceContext);



	//// Clear depth buffer to 1.0f and stencil buffer to 0.
	mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	
	mQuadTree->RenderSnow(mDeviceContext, mParticleShader, mCamera);
	


	
	for (int i = 0; i < BUFFER_COUNT-2; i++)
	{
		mRTQ[i]->SetAsObjectToBeDrawn(mDeviceContext, 0);
		mTexShader->Render(mDeviceContext, mRTQ[i]->GetIndexCount(0), mRTQ[i]->GetWorldMatrix(), mCamera->GetViewMatrix(), mCamera->GetProjMatrix(), mCamera->GetForward(), mDeferredBuffer->GetShaderResourceView(i));
	}
	mRTQ[2]->SetAsObjectToBeDrawn(mDeviceContext, 0);
	mTexShader->Render(mDeviceContext, mRTQ[2]->GetIndexCount(0), mRTQ[2]->GetWorldMatrix(), mCamera->GetViewMatrix(), mCamera->GetProjMatrix(), mCamera->GetForward(), mDeferredBuffer->GetLightSRV());

	
	// Present the back buffer to the screen


	hr = mSwapChain->Present(0, 0);
	if (FAILED(hr))
	{
		MessageBox(0, L"Failed to swap back buffer", 0, 0);
		return;
	}

	

	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	if ((mTimer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt;
		float mspf = 1000.0f / fps;

		std::wostringstream outs;
		outs.precision(6);
		outs << mMainWndCaption << L"Fps: " << fps << L" Total Time: " << mspf << L" ms" << "Terrain: " << counter;

		
		for (UINT i = 0; i < GPU_TIMER_COUNT; i++)
		{
			double t = mGPUTimer[i].GetTime(mDeviceContext);
			outs << " GPU Time" << i << ": " << t;
		}
		SetWindowText(mhMainWnd, outs.str().c_str());


		frameCnt = 0;
		timeElapsed += 1.0f;
	}


}

void InitDirect3DApp::OnMouseDown(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{

		//mCamera->MoveForward(true);
		
	}
	else if ((btnState & MK_RBUTTON) != 0)
	{
		/*XMFLOAT3 pos = mCamera->GetPosition();
		pos.z -= 1;
		mCamera->SetPosition(pos);*/

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
