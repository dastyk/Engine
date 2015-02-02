#ifndef _POINTLIGHTCLASS_H_
#define _POINTLIGHTCLASS_H_

#pragma once

#include "LightClass.h"

class PointLightClass : public LightClass
{
public:
	PointLightClass();
	PointLightClass(XMFLOAT3 color, XMFLOAT3 pos, float Radius);
	~PointLightClass();

	float GetRadius();
	void SetRadius(float radius);


private:
	float mRadius;
};

#endif