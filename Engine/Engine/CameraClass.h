#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_

#pragma once

#include "PositionClass.h"
#include <DirectXCollision.h>

class CameraClass : public PositionClass
{
public:
	CameraClass();
	~CameraClass();

	void SetProjMatrix(float FoV, float AspectRatio, float nearP, float farP);

	void SetRotation(XMFLOAT3& rot);

	XMFLOAT4X4 GetViewMatrix()const;
	XMFLOAT4X4 GetProjMatrix()const;
	BoundingFrustum GetBoundingFrustum()const;


	void CalcViewMatrix();
	
private:
	XMFLOAT4X4 mViewMatrix;
	XMFLOAT4X4 mProjMatrix;
	XMFLOAT4X4 mInvViewMatrix;

	float mFoV, mAspectRatio, mNearPlane, mFarPlane;

	BoundingFrustum mFrustum;
};

#endif