#include "QuadTree.h"


QuadTree::QuadTree(int pointCount, const XMFLOAT3* pPoints, const unsigned long* pIndices, int indexCount, int wOffset, int dOffset,int width, int depth, int fWidth,int fDepth)
{
	AABB::createFromPoints(pointCount, pPoints);
	
	mIndexCount = indexCount;
	mWOffset = wOffset;
	mDOffset = dOffset;
	mWidth = width;
	mDepth = depth;
	mFWidth = fWidth;
	mFDepth = fDepth;

	QuadTree::createChildren(pIndices);

}

QuadTree::~QuadTree()
{
	for (int i = 0; i < 4; i++)
	{
		if (mChildren[i])
		{
			delete mChildren[i];
			mChildren[i] = 0;
		}
	}
	if (mIndices)
	{
		delete mIndices;
		mIndices = 0;
	}
}

void QuadTree::createChildren(const unsigned long* pIndices)
{
	XMFLOAT3 c = mBox.Center;
	XMFLOAT3 e = mBox.Extents;

	if (mIndexCount <= QUAD_SIZE_MIN)
	{
		for (int i = 0; i < 4; i++)
			mChildren[i] = nullptr;

		createIndexArray(pIndices);
	}
	else
	{
		// Middle points
		XMFLOAT3 p1 = XMFLOAT3(c.x, c.y + e.y, c.z);
		XMFLOAT3 p2 = XMFLOAT3(c.x, c.y - e.y, c.z);

		// Middle Top points
		XMFLOAT3 p3 = XMFLOAT3(c.x, c.y + e.y, c.z + e.z);
		XMFLOAT3 p4 = XMFLOAT3(c.x, c.y - e.y, c.z + e.z);

		// Middle Right points
		XMFLOAT3 p5 = XMFLOAT3(c.x + e.x, c.y + e.y, c.z);
		XMFLOAT3 p6 = XMFLOAT3(c.x + e.x, c.y - e.y, c.z);

		// Middle Bottom points
		XMFLOAT3 p7 = XMFLOAT3(c.x, c.y + e.y, c.z - e.z);
		XMFLOAT3 p8 = XMFLOAT3(c.x, c.y - e.y, c.z - e.z);

		// Middle left points
		XMFLOAT3 p9 = XMFLOAT3(c.x - e.x, c.y + e.y, c.z);
		XMFLOAT3 p10 = XMFLOAT3(c.x - e.x, c.y - e.y, c.z);

		// Top Left points
		XMFLOAT3 p11 = XMFLOAT3(c.x - e.x, c.y + e.y, c.z + e.z);
		XMFLOAT3 p12 = XMFLOAT3(c.x - e.x, c.y - e.y, c.z + e.z);

		// Top Right points
		XMFLOAT3 p13 = XMFLOAT3(c.x + e.x, c.y + e.y, c.z + e.z);
		XMFLOAT3 p14 = XMFLOAT3(c.x + e.x, c.y - e.y, c.z + e.z);

		// Bottom Left points
		XMFLOAT3 p15 = XMFLOAT3(c.x - e.x, c.y + e.y, c.z - e.z);
		XMFLOAT3 p16 = XMFLOAT3(c.x - e.x, c.y - e.y, c.z - e.z);

		// Bottom Right points
		XMFLOAT3 p17 = XMFLOAT3(c.x + e.x, c.y + e.y, c.z - e.z);
		XMFLOAT3 p18 = XMFLOAT3(c.x + e.x, c.y - e.y, c.z - e.z);

		int indexCount = mIndexCount / 4;
		int wOffset = mWOffset;
		int dOffset = mDOffset + mDepth / 2;
		int width = mWidth / 2;
		int depth = mDepth / 2;

		// Top left box
		XMFLOAT3 b0[8] = { p1, p2, p3, p4, p9, p10, p11, p12 };
		mChildren[0] = new QuadTree(8, b0,pIndices, indexCount,wOffset, dOffset,width,depth,mFWidth, mFDepth);

		wOffset = mWOffset + mWidth / 2;
		dOffset = mDOffset + mDepth / 2;

		// Top Right box
		XMFLOAT3 b1[8] = { p1, p2, p3, p4, p5, p6, p13, p14 };
		mChildren[1] = new QuadTree(8, b1, pIndices, indexCount, wOffset, dOffset, width, depth, mFWidth, mFDepth);

		wOffset = mWOffset;
		dOffset = mDOffset;

		// Bottom Left box
		XMFLOAT3 b2[8] = { p1, p2, p7, p8, p9, p10, p15, p16 };
		mChildren[2] = new QuadTree(8, b2, pIndices, indexCount, wOffset, dOffset, width, depth, mFWidth, mFDepth);

		wOffset = mWOffset + mWidth / 2;
		dOffset = mDOffset;

		// Bottom Right box
		XMFLOAT3 b3[8] = { p1, p2, p7, p8, p5, p6, p17, p18 };
		mChildren[3] = new QuadTree(8, b3, pIndices, indexCount, wOffset, dOffset, width, depth, mFWidth, mFDepth);
	}
}

void QuadTree::createIndexArray(const unsigned long* pIndices)
{
	mIndices = new unsigned long[QUAD_SIZE_MIN];
	int count = 0;
	for (int j = 0; j < mDepth; j++)
	{
		for (int i = 0; i < mWidth; i++)
		{
			int d = ((mDOffset + j)*mFWidth + (mWOffset + i)) * 6;
			for (int u = 0; u < 6; u++)
			{
				mIndices[count] = pIndices[d + u];
				count++;
			}
		}
	}
}

bool QuadTree::GetIndexArray(int& indexCount, unsigned long* pIndices, BoundingFrustum& frustum)
{
	if (Intersect(frustum))
	{
		if (mChildren[0])
		{
			int iCount = 0;
			for (int i = 0; i < 4; i++)
			{
				if (mChildren[i]->GetIndexArray(indexCount, pIndices, frustum))
				{
					iCount += indexCount;
					
				}
				indexCount = iCount;
			}

			
		}
		else
		{
			
			for (int i = 0; i < mIndexCount; i++)
			{
				pIndices[indexCount + i] = mIndices[i];
			}
			indexCount = mIndexCount;

		}
		return true;
	}
	indexCount = 0;
	pIndices = 0;
	return false;
}
