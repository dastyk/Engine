#pragma once
#ifndef _INITDIRECT3DAPP_H_
#define _INITDIRECT3DAPP_H_

#pragma once

#define GPU_TIMER_COUNT 4

#include "D3DApp.h"
#include <crtdbg.h>

#include "ColorShaderClass.h"
#include "TextureShaderClass.h"
#include "ObjectClass.h"
#include "CameraClass.h"

#include "LightShaderClass.h"

#include "LightObjectClass.h"
#include "PointLightClass.h"


#include "FogClass.h"

#include "TerrainClass.h"
#include "TerrainShaderClass.h"

#include "ParticleShaderClass.h"
#include "FireworkEffect.h"

#include <time.h>


#include "DeferredBufferClass.h"
#include "DeferredShaderClass.h"

#include "ShadowMapClass.h"


#include "BoundingBoxShader.h"

#include "DeferredComputeShaderClass.h"

#include "GPUTimerClass.h"

class InitDirect3DApp : public D3DApp
{
public:
	InitDirect3DApp(HINSTANCE hInstance);
	~InitDirect3DApp();

	bool Init();

	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();
	void handleInput();

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

	


private:
	ShaderClass* mColorShader;
	TextureShaderClass* mTexShader;
	LightShaderClass* mLightShader;
	TerrainShaderClass* mTerrainShader;

	ModelClass* mModel;
	ModelClass* mQuadModel;
	ObjectClass* mRTQ[BUFFER_COUNT];

	ObjectClass** mObject;
	UINT mNRofObjects;

	LightObjectClass* mSun;

	TerrainClass* mTerrainModel;
	ObjectClass* mTerrain;
	QuadTree* mQuadTree;


	ParticleShaderClass* mParticleShader;
	FireworkEffect* mFirework;

	FogClass* mDrawDistFog;

	CameraClass* mCamera;

	POINT mLastMousePos;


	DeferredBufferClass* mDeferredBuffer;
	DeferredShaderClass* mDeferredShader;

	vector< PointLightClass*> mPointLight;
	UINT mLightCount;

	ShadowMapClass* mShadowmapShader;
	BoundingBoxShader* mBoundingBoxShader;



	DeferredComputeShaderClass* mDCShader;

#if TIME
		GPUTimerClass mGPUTimer[GPU_TIMER_COUNT];
#endif
};

#endif