#ifndef _SPECULARLIGHTCLASS_H_
#define _SPECULARLIGHTCLASS_H_

#pragma once
#include "LightClass.h"
class SpecularLightClass :
	public LightClass
{
public:
	SpecularLightClass();
	SpecularLightClass(XMFLOAT3& dir, XMFLOAT3& color, XMFLOAT3& pos, float shinyPower);
	~SpecularLightClass();

	float GetShinyPower()const;

private:
	float mShinyPower;
};

#endif