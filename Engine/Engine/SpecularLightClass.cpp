#include "SpecularLightClass.h"


SpecularLightClass::SpecularLightClass() : LightClass()
{
	mShinyPower = 0;
}
SpecularLightClass::SpecularLightClass(XMFLOAT3& dir, XMFLOAT3& color, XMFLOAT3& pos, float shinyPower) : LightClass(dir, color, pos)
{
	mShinyPower= shinyPower;
}

SpecularLightClass::~SpecularLightClass()
{
}

float SpecularLightClass::GetShinyPower()const
{
	return mShinyPower;
}