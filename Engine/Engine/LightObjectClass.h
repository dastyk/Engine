#ifndef _LIGHTOBJECTCLASS_H_
#define _LIGHTOBJECTCLASS_H_

#pragma once

#include "LightClass.h"
#include <DirectXMath.h>

using namespace DirectX;

class LightObjectClass
{
public:
	LightObjectClass();
	LightObjectClass(LightClass* pAmbient, LightClass* pDiffuse, LightClass* pSpecular);
	virtual ~LightObjectClass();


	XMFLOAT3 GetLighting(XMFLOAT3& pos, XMFLOAT3& normal);

	LightClass* GetAmbientLight();
	LightClass* GetDiffuseLight();
	LightClass* GetSpecularLight();

protected:
	LightClass* mAmbient;
	LightClass* mDiffuse;
	LightClass* mSpecular;
};

#endif