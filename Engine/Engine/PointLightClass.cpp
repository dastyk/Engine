#include "PointLightClass.h"


PointLightClass::PointLightClass()
{
}
PointLightClass::PointLightClass(XMFLOAT3 color, XMFLOAT3 pos, float Radius) : LightClass(XMFLOAT3(0, 0, 0), color, pos)
{
	mRadius = Radius;
}

PointLightClass::~PointLightClass()
{
}

float PointLightClass::GetRadius()
{
	return mRadius;
}

void PointLightClass::SetRadius(float radius)
{
	mRadius = radius;
}