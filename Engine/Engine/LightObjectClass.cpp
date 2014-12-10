#include "LightObjectClass.h"


LightObjectClass::LightObjectClass()
{
	mAmbient = new LightClass(XMFLOAT3(0, 0, 0), XMFLOAT3(0.3, 0.3, 0.3), XMFLOAT3(0, 0, 0));
	mDiffuse = new LightClass(XMFLOAT3(0, 0, 1), XMFLOAT3(0.5, 0.5, 0.5), XMFLOAT3(0, 0, 0));
	mSpecular = new LightClass(XMFLOAT3(0, 0, 1), XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, -1));
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