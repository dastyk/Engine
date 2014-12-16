#ifndef _OBB_H_
#define _OBB_H_

#pragma once

#include <DirectXCollision.h>

using namespace DirectX;

class OBB
{
public:
	OBB();
	virtual ~OBB();

	void createFromPoints(int pointCount, const XMFLOAT3* pPoints);

	bool Intersect(XMVECTOR origin, XMVECTOR direction, float& dist)const;
	bool InterSect(const BoundingOrientedBox&)const;

protected:

	BoundingOrientedBox mBox;
};

#endif