#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#pragma once

#define QUAD_TREE_CHILDREN_COUNT 4

#include "ObjectClass.h"


class QuadTree
{
public:
	QuadTree();
	~QuadTree();


	bool Init(ID3D11Device* pDevice, int pointCount, const XMFLOAT3* pPoints, size_t stride, int indexCount, unsigned long* pIndices, int width, int depth);


private:
	bool createChildren();

private:
	ObjectClass** mObjects;
	UINT mObjectCount;

	ID3D11Buffer* mIndexBuffer;
	BoundingBox mBox;
	QuadTree* mChildren[QUAD_TREE_CHILDREN_COUNT];
};

#endif