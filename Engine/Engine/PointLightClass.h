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

	XMFLOAT4X4 GetViewMatrix()const;
	XMFLOAT4X4 GetProjMatrix()const;

	void CalcViewMatrix();
	void SetProjMatrix(float FoV, float AspectRatio, float nearP, float farP);

private:
	float mRadius;

	XMFLOAT4X4 mViewMatrix;
	XMFLOAT4X4 mProjMatrix;
};

#endif