#include "AABB.h"


AABB::AABB()
{
}


AABB::~AABB()
{
}

void AABB::createFromPoints(int pointCount, const XMFLOAT3* pPoints)
{
	BoundingBox::CreateFromPoints(mBox, pointCount, pPoints, 12);
}


bool AABB::Intersect(XMVECTOR origin, XMVECTOR direction, float& dist)const
{
	return mBox.Intersects(origin, direction, dist);
}

bool AABB::Intersect(const BoundingBox& boundingBox)const
{
	return mBox.Intersects(boundingBox);
}