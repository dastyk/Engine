#ifndef _AABB_H_
#define _AABB_H_

#pragma once

#include <DirectXCollision.h>

using namespace DirectX;

class AABB
{
public:
	AABB();
	virtual ~AABB();

	void createFromPoints(int pointCount, const XMFLOAT3* pPoints);

	bool Intersect(XMVECTOR origin, XMVECTOR direction, float& dist)const;
	bool Intersect(const BoundingBox&)const;

protected:

	BoundingBox mBox;
};

#endif