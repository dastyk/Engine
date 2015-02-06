#pragma once
#ifndef _INITDIRECT3DAPP_H_
#define _INITDIRECT3DAPP_H_

#pragma once

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
#include "SnowEffect.h"

#include <time.h>


#include "DeferredBufferClass.h"
#include "DeferredShaderClass.h"

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
	ShaderClass* mTexShader;
	LightShaderClass* mLightShader;
	TerrainShaderClass* mTerrainShader;

	ModelClass* mModel;
	ModelClass* mQuadModel;
	ObjectClass* mRTQ[BUFFER_COUNT];

	ObjectClass* mObject;

	LightObjectClass* mSun;

	TerrainClass* mTerrainModel;
	ObjectClass* mTerrain;

	ParticleShaderClass* mParticleShader;
	FireworkEffect* mFirework;
	SnowEffect* mSnow;

	FogClass* mDrawDistFog;

	CameraClass* mCamera;

	POINT mLastMousePos;


	DeferredBufferClass* mDeferredBuffer;
	DeferredShaderClass* mDeferredShader;

	PointLightClass** mPointLight;
	UINT mLightCount;
};

#endif