#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#pragma once
#define QUAD_SIZE_MIN 98304
#include "AABB.h"
#include <d3d11.h>
#include "ModelClass.h"

class QuadTree : public AABB
{
public:
	QuadTree(int pointCount, const XMFLOAT3* pPoints, int indexCount, unsigned long* pIndices,int width, int depth);
	QuadTree(XMVECTOR p1, XMVECTOR p2, int indexCount, unsigned long* pIndices, int wOffset, int dOffset, int width, int depth, int fWidth, int fDepth);
	~QuadTree();

	bool GetIndexArray(int& indexCount, unsigned long* pIndices, BoundingFrustum& frustum);

private:
	void createChildren(int width, int depth, int fWidth, int fDepth, int wOffset, int dOffset, unsigned long* pIndices);

private:
	QuadTree* mChildren[4];

	int mIndexCount;
	unsigned long* mIndices;
	ID3D11Buffer* mIndexBuffer;

};

#endif