#include "DiffuseLightClass.h"


DiffuseLightClass::DiffuseLightClass() : LightClass()
{
}

DiffuseLightClass::DiffuseLightClass(XMFLOAT3& dir, XMFLOAT3& color, XMFLOAT3& pos) : LightClass(dir, color, pos)
{
}

DiffuseLightClass::~DiffuseLightClass()
{
}
