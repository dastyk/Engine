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
	mSnow = 0;

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

	mDrawDistFog = 0;

	mCamera = 0;


	mDeferredBuffer = nullptr;
	mDeferredShader = nullptr;

	mEnable4xMsaa = true;
	m4xMsaaQuality = 1;


	mPointLight = 0;
	mLightCount = 0;

	srand(time(NULL));

	mShadowmapShader = 0;
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
	if (mSnow)
	{
		delete mSnow;
		mSnow = 0;
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
	XMFLOAT3  pos(128, 0, 118.0f);// pos(0.0f, 0.0f, 0.0f);
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

	mNRofObjects = 100;
	mObject = new ObjectClass*[mNRofObjects];
	if (!mObject)
		return false;
	for (UINT i = 0; i < mNRofObjects; i++)
	{
		mObject[i] = new ObjectClass(mModel);
		TransformationClass* t = mObject[i]->GetTransformation();
		t->SetPosition(XMFLOAT3(128.0f + rand() % 100 - 50, 0, 128.0f + rand() % 100 - 50));
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

	mSnow = new SnowEffect();
	if (!mSnow)
		return false;
	mSnow->SetPlayerPos(mCamera->GetPosition());
	result = mSnow->Init(mDevice);
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

	mLightCount = 2;
	mPointLight = new PointLightClass*[mLightCount]; 
	if (!mPointLight)
		return false;


	mPointLight[1] = new PointLightClass(XMFLOAT3(1, 0, 0), XMFLOAT3(0, 0, 0), 50);
	mPointLight[0] = new PointLightClass(XMFLOAT3(0.5, 0.5, 0.5), XMFLOAT3(128, 115, 155), 50000);
	mPointLight[0]->SetLightDir(XMFLOAT3(0, -1, -1));
	mPointLight[0]->SetProjMatrix(mFoV, AspectRatio(), mNearPlane, mFarPlane);

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
	TransformationClass* temp = 0;
	XMFLOAT3 rot;
	XMFLOAT3 pos;
	for (UINT i = 0; i < mNRofObjects; i++)
	{
		temp = mObject[i]->GetTransformation();
		rot = temp->GetRotation();
		pos = temp->GetPosition();
		rot.y += dt*100;
		pos.y = mTerrainModel->getHeightAtPoint(pos) + 0.5;


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
	mCamera->SetPosition(pos);

	/*std::wostringstream outs;
	outs.precision(6);
	outs << mMainWndCaption << mFirework->GetAliveParticles() + mSnow->GetAliveParticles();
	SetWindowText(mhMainWnd, outs.str().c_str());*/

	mCamera->SetUpdateTime(dt);

	
	//mFirework->Update(dt);

	/*mSnow->SetPlayerPos(mCamera->GetPosition());
	mSnow->Update(dt);*/

	mCamera->CalcViewMatrix();

	
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
}

void InitDirect3DApp::DrawScene()
{
	HRESULT hr;
	bool result;

	assert(mDeviceContext);
	assert(mSwapChain);

	BoundingFrustum f = mCamera->GetBoundingFrustum();
	BoundingFrustum f2 = mPointLight[0]->GetBoundingFrustum();
	UINT count = 0;

	mShadowmapShader->SetRTV(mDeviceContext);
	mShadowmapShader->ClearRTV(mDeviceContext);
	
	for (UINT i = 0; i < mNRofObjects; i++)
	{
		if (mObject[i]->SetAsObjectToBeDrawn(mDeviceContext, f2, 1))
		{
			result = mShadowmapShader->CreateShadowMap(mDeviceContext, mObject[i], mPointLight[0]);
			if (!result)
			{
				MessageBox(0, L"Failed to Render Shaders", 0, 0);
				return;
			}
		}



	}

	mShadowmapShader->UnbindRTV(mDeviceContext);

	mDeferredBuffer->SetRenderTargets(mDeviceContext);
	mDeferredBuffer->ClearRenderTargets(mDeviceContext, 0.0f, 0.0f, 0.0f, 0.0f);

	for (UINT i = 0; i < mNRofObjects; i++)
	{
		if (mObject[i]->SetAsObjectToBeDrawn(mDeviceContext, f, 0))
		{
			//mObject->SetAsObjectToBeDrawn(mDeviceContext);
			//mDeferredShader->RenderDeferred(mDeviceContext, mObject, mCamera);

			

			mDeferredShader->RenderDeferred(mDeviceContext, mObject[i], mCamera);

			


			count++;
		}
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
		outs << mMainWndCaption << L"Fps: " << fps << L" Frame Time: " << mspf << L" ms"<< " Rendering: " << count << " models";;
		SetWindowText(mhMainWnd, outs.str().c_str());


		frameCnt = 0;
		timeElapsed += 1.0f;
	}

	//mTerrain->SetAsObjectToBeDrawn(mDeviceContext);
	//result = mTerrainShader->RenderDeferred(
	//	mDeviceContext,
	//	mTerrain,
	//	mCamera);

	//if (!result)
	//{
	//	MessageBox(0, L"Failed to Render Shaders", 0, 0);
	//	return;
	//}

	mTerrain->SetAsObjectToBeDrawn(mDeviceContext);
	result = mTerrainShader->RenderShadowsDeferred(
		mDeviceContext,
		mTerrain,
		mCamera,
		mPointLight[0],
		mShadowmapShader->GetShaderResourceView());

	if (!result)
	{
		MessageBox(0, L"Failed to Render Shaders", 0, 0);
		return;
	}

	mDeferredBuffer->UnsetRenderTargets(mDeviceContext);

	// Clear back buffer blue.
	float clearColor[] = { 0.4f, 0.4f, 0.9f, 1.0f };
	mDeviceContext->ClearRenderTargetView(mRenderTargetView, clearColor);

	mDeferredShader->Render(
		mDeviceContext, 
		mDeferredBuffer,
		mObject[0],
		mCamera,
		mPointLight,
		mLightCount,
		mDrawDistFog);


	// Clear depth buffer to 1.0f and stencil buffer to 0.
	mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	



	

	/*mFirework->render(mDeviceContext);

	result = mParticleShader->Render(mDeviceContext, mFirework, mCamera);
	if (!result)
	{
		MessageBox(0, L"Failed to Render Shaders", 0, 0);
		return;
	}*/

	/*mSnow->render(mDeviceContext);

	result = mParticleShader->Render(mDeviceContext, mSnow, mCamera);
	if (!result)
	{
		MessageBox(0, L"Failed to Render Shaders", 0, 0);
		return;
	}
	*/
	/*mTerrain->SetAsObjectToBeDrawn(mDeviceContext);
	
	result = mTerrainShader->Render(
		mDeviceContext,
		mTerrain,
		mCamera,
		mSun,
		mObject->GetMaterial(),
		mDrawDistFog);
	
	if (!result)
	{
		MessageBox(0, L"Failed to Render Shaders", 0, 0);
		return;
	}
	*/
	/*mObject->SetAsObjectToBeDrawn(mDeviceContext);

	result = mLightShader->Render(
		mDeviceContext,
		mObject,
		mCamera,
		mPointLight,
		mLightCount,
		mDrawDistFog);*/
	////result = mLightShader->RenderAnimated(
	////		mDeviceContext,
	////		mObject,
	////		mCamera,
	////		mPointLight,
	////		mLightCount,
	////		mDrawDistFog);


	//if (!result)
	//{
	//	MessageBox(0, L"Failed to Render Shaders", 0, 0);
	//	return;
	//}


	for (int i = 0; i < BUFFER_COUNT-1; i++)
	{
		mRTQ[i]->SetAsObjectToBeDrawn(mDeviceContext);
		mTexShader->Render(mDeviceContext, mRTQ[i]->GetIndexCount(), mRTQ[i]->GetWorldMatrix(), mCamera->GetViewMatrix(), mCamera->GetProjMatrix(), mDeferredBuffer->GetShaderResourceView(i));
	}
	mRTQ[2]->SetAsObjectToBeDrawn(mDeviceContext);
	mTexShader->Render(mDeviceContext, mRTQ[2]->GetIndexCount(), mRTQ[2]->GetWorldMatrix(), mCamera->GetViewMatrix(), mCamera->GetProjMatrix(), mShadowmapShader->GetShaderResourceView());

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
