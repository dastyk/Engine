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

	void createFromPoints(int pointCount, const XMFLOAT3* pPoints, size_t stride);
	void createFromPoints(XMVECTOR p1, XMVECTOR p2);


	bool Intersect(XMVECTOR origin, XMVECTOR direction, float& dist)const;
	bool Intersect(const BoundingBox&)const;
	bool Intersect(const BoundingFrustum&)const;
	BoundingBox GetBoundingBox();

protected:

	BoundingBox mBox;
};

#endif