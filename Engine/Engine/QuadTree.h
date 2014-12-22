#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#pragma once

#define QUAD_SIZE_MIN 24576

#include "AABB.h"

class QuadTree : public AABB
{
public:
	QuadTree(int pointCount, const XMFLOAT3* pPoints, const unsigned long* pIndices, int indexCount, int wOffset, int dOffset, int width, int depth, int fWidth, int fDepth);
	~QuadTree();


	bool GetIndexArray(int& indexCount, unsigned long* ppIndices, BoundingFrustum& frustum);


private:
	void createChildren(const unsigned long* pIndices);

	void createIndexArray(const unsigned long* pIndices);

private:
	QuadTree* mChildren[4];

	unsigned long* mIndices;
	int mIndexCount;
	int mWOffset, mDOffset;
	int mWidth, mDepth;
	int mFWidth, mFDepth;

};

#endif