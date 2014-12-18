#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#pragma once

#include "AABB.h"

class QuadTree : public AABB
{
public:
	QuadTree(int pointCount, const XMFLOAT3* pPoints, float minSize);
	~QuadTree();

	

private:
	void createChildren(float minSize);

private:
	QuadTree* mChildren[4];

};

#endif