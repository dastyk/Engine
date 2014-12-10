#include "AmbientLightClass.h"


AmbientLightClass::AmbientLightClass() : LightClass()
{
}

AmbientLightClass::AmbientLightClass(XMFLOAT3& color) : LightClass(XMFLOAT3(0, 0, 0), color, XMFLOAT3(0, 0, 0))
{
}

AmbientLightClass::~AmbientLightClass()
{
}
