#ifndef _OBB_H_
#define _OBB_H_

#pragma once

#define EPSILON 0.000001


#include "Ray.h"

#include <DirectXMath.h>

using namespace DirectX;

class OBB
{
public:
	OBB();
	virtual ~OBB();

	bool IntersectRay(Ray*, float&;

protected:
	XMFLOAT3 mPos;
	XMFLOAT3 mBaseVector[3];
	float mHalfLenght[3];
};

#endif