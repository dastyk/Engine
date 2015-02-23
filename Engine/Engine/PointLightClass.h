#ifndef _POINTLIGHTCLASS_H_
#define _POINTLIGHTCLASS_H_

#pragma once

#include "LightClass.h"
#include <DirectXCollision.h>
using namespace DirectX;

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

	void SetLightPos(XMFLOAT3&);

	void CalcViewMatrix();
	void SetProjMatrix(float FoV, float AspectRatio, float nearP, float farP);

	BoundingFrustum GetBoundingFrustum()const;
	BoundingSphere GetBoundingSphere()const;
private:
	float mRadius;

	XMFLOAT4X4 mViewMatrix;
	XMFLOAT4X4 mProjMatrix;
	XMFLOAT4X4 mInvViewMatrix;

	BoundingFrustum mFrustum;

	BoundingSphere mSphere;
};

#endif