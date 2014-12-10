#include "LightClass.h"


LightClass::LightClass()
{
	mLightDir = XMFLOAT3(0, 0, 1);
	mLightColor = XMFLOAT3(1, 1, 1);
	mLightPos = XMFLOAT3(0, 0, 0);
}

LightClass::LightClass(XMFLOAT3& dir, XMFLOAT3& color, XMFLOAT3& pos)
{
	mLightDir = dir;
	mLightColor = color;
	mLightPos = pos;
}

LightClass::~LightClass()
{
}

void LightClass::SetLightDir(XMFLOAT3& dir)
{
	XMVECTOR vDir = XMLoadFloat3(&dir);
	XMStoreFloat3(&mLightDir, XMVector3Normalize(vDir));
}

void LightClass::SetLightColor(XMFLOAT3& color)
{
	mLightColor = color;
}

void LightClass::SetLightPos(XMFLOAT3& pos)
{
	mLightPos = pos;
}

XMFLOAT3 LightClass::GetLightDir()const
{
	return (XMFLOAT3)mLightDir;
}

XMFLOAT3 LightClass::GetLightColor()const
{
	return (XMFLOAT3)mLightColor;
}

XMFLOAT3 LightClass::GetLightPos()const
{
	return (XMFLOAT3)mLightPos;
}
