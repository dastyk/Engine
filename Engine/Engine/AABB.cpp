#include "AABB.h"


AABB::AABB()
{
}


AABB::~AABB()
{
}

void AABB::createFromPoints(int pointCount, const XMFLOAT3* pPoints)
{
	BoundingBox::CreateFromPoints(mBox, pointCount, pPoints, sizeof(XMFLOAT3));
}

void AABB::createFromPoints(XMVECTOR p1, XMVECTOR p2)
{
	BoundingBox::CreateFromPoints(mBox, p1, p2);
}

bool AABB::Intersect(XMVECTOR origin, XMVECTOR direction, float& dist)const
{
	return mBox.Intersects(origin, direction, dist);
}

bool AABB::Intersect(const BoundingBox& boundingBox)const
{
	return mBox.Intersects(boundingBox);
}

bool AABB::Intersect(const BoundingFrustum& frustum)const
{
	return frustum.Contains(mBox);
}
