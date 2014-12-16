#include "OBB.h"


OBB::OBB()
{

}


OBB::~OBB()
{
}

void OBB::createFromPoints(int pointCount, const XMFLOAT3* pPoints)
{
	BoundingOrientedBox::CreateFromPoints(mBox, pointCount, pPoints, 12);
}


bool OBB::Intersect(XMVECTOR origin, XMVECTOR direction, float& dist)const
{
	return mBox.Intersects(origin, direction, dist);
}

bool OBB::InterSect(const BoundingOrientedBox& boundingBox)const
{
	return mBox.Intersects(boundingBox);
}