#include "FogClass.h"


FogClass::FogClass()
{
}

FogClass::FogClass(XMFLOAT3& pos, XMFLOAT3& color, float range)
{
	mPosition = pos;
	mColor = color;
	mRange = range;
}

FogClass::~FogClass()
{
}

void FogClass::SetPosition(XMFLOAT3& pos)
{
	mPosition = pos;
}

void FogClass::SetColor(XMFLOAT3& color)
{
	mColor = color;
}

void FogClass::SetRange(float& range)
{
	mRange = range;
}

XMFLOAT3 FogClass::GetPosition()const
{
	return mPosition;
}
XMFLOAT3 FogClass::GetColor()const
{
	return mColor;
}
float FogClass::GetRange()const
{
	return mRange;
}