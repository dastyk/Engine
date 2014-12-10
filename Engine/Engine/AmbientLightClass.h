#ifndef _AMBIENTLIGHTCLASS_H_
#define _AMBIENTLIGHTCLASS_H_


#pragma once
#include "LightClass.h"
class AmbientLightClass :
	public LightClass
{
public:
	AmbientLightClass();
	AmbientLightClass(XMFLOAT3& color);
	~AmbientLightClass();
};

#endif