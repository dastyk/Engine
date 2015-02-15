#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#pragma once

#define QUAD_TREE_CHILDREN_COUNT 4
#define QUAD_SIZE_MIN 98304
#define QUAD_TREE_QUADS_PER_LEAF 64


#include "ObjectClass.h"


class QuadTree
{
public:
	QuadTree();	
	~QuadTree();

	bool Init(UINT pointCount, const XMFLOAT3* pPoints, size_t stride, UINT indexCount);
	
	


private:
	bool createChildren();
	bool Init(XMVECTOR p1, XMVECTOR p2, QuadTree* pParent, UINT indexCount);

private:
	ObjectClass** mObjects;
	UINT mObjectCount;

	BoundingBox mBox;
	QuadTree* mChildren[QUAD_TREE_CHILDREN_COUNT];
	QuadTree* mParent;

	UINT mIndexStart;
	UINT mIndexCount;

};

#endif