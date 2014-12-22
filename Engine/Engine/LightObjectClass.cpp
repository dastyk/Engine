#include "LightObjectClass.h"


LightObjectClass::LightObjectClass()
{
	mAmbient = new LightClass(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.3f, 0.3f, 0.3f), XMFLOAT3(0.0f, 0.0f, 0.0f));
	mDiffuse = new LightClass(XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3(0.6f, 0.6f, 0.6f), XMFLOAT3(0.0f, 0.0f, 0.0f));
	mSpecular = new LightClass(XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f));
}

LightObjectClass::LightObjectClass(LightClass* pAmbient, LightClass* pDiffuse, LightClass* pSpecular)
{
	mAmbient = pAmbient;
	mDiffuse = pDiffuse;
	mSpecular = pSpecular;
}

LightObjectClass::~LightObjectClass()
{
	if (mAmbient)
	{
		delete mAmbient;
		mAmbient = 0;
	}
	if (mDiffuse)
	{
		delete mDiffuse;
		mDiffuse = 0;
	}
	if (mSpecular)
	{
		delete mSpecular;
		mSpecular = 0;
	}

}

XMFLOAT3 LightObjectClass::GetLighting(XMFLOAT3& pos, XMFLOAT3& normal)
{
	return XMFLOAT3(0, 0, 0);
}

LightClass* LightObjectClass::GetAmbientLight()
{
	return mAmbient;
}

LightClass* LightObjectClass::GetDiffuseLight()
{
	return mDiffuse;
}

LightClass* LightObjectClass::GetSpecularLight()
{
	return mSpecular;
}