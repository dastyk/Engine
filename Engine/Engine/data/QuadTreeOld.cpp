#include "QuadTreeOld.h"


QuadTree::QuadTree(int pointCount, const XMFLOAT3* pPoints, int indexCount, unsigned long* pIndices, int width, int depth)
{
	AABB::createFromPoints(pointCount, pPoints);

	mIndexCount = indexCount;

	mIndices = new unsigned long[mIndexCount];
	for (int i = 0; i < mIndexCount; i++)
	{
		mIndices[i] = pIndices[i];
	}

	QuadTree::createChildren(width, depth, width, depth, 0, 0, pIndices);
}

QuadTree::QuadTree(XMVECTOR p1, XMVECTOR p2, int indexCount, unsigned long* pIndices, int wOffset, int dOffset, int width, int depth, int fWidth, int fDepth)
{
	AABB::createFromPoints(p1, p2);

	mIndexCount = indexCount / 4;
	int nWidth = width / 2;
	int nDepth = depth / 2;

	if (mIndexCount <= QUAD_SIZE_MIN)
	{
		mIndices = new unsigned long[mIndexCount];

		int count = 0;
		for (int j = 0; j < nDepth; j++)
		{
			for (int i = 0; i < nWidth; i++)
			{
				int d = ((dOffset + j)*fWidth + (wOffset + i)) * 6;
				for (int u = 0; u < 6; u++)
				{
					unsigned long test = pIndices[d + u];
					mIndices[count] = test;
					count++;
				}
			}
		}
	}
	else
		createChildren(nWidth, nDepth,fWidth,fDepth, wOffset, dOffset, pIndices);


}

QuadTree::~QuadTree()
{
	for (int i = 0; i < 4; i++)
	{
		if (mChildren[i])
			delete mChildren[i];
	}
	if (mIndices)
	{
		delete mIndices;
		mIndices = 0;
	}
}

void QuadTree::createChildren(int width, int depth, int fWidth, int fDepth, int wOffset, int dOffset, unsigned long* pIndices)
{
	XMFLOAT3 c = mBox.Center;
	XMFLOAT3 e = mBox.Extents;

	int nwOffset = wOffset;
	int ndOffset = dOffset + depth / 2;

	// Top Left Box
	mChildren[0] = new QuadTree(
		XMVectorSet(c.x - e.x, c.y - e.y, c.z, 0),
		XMVectorSet(c.x, c.y + e.y, c.z + e.z, 0),
		mIndexCount,
		pIndices,
		nwOffset,
		ndOffset,
		width,
		depth,
		fWidth,
		fDepth);

	nwOffset = wOffset + width / 2;
	ndOffset = dOffset + depth / 2;

	// Top Right Box
	mChildren[1] = new QuadTree(
		XMVectorSet(c.x, c.y - e.y, c.z, 0),
		XMVectorSet(c.x + e.x, c.y + e.y, c.z + e.y, 0),
		mIndexCount,
		pIndices,
		nwOffset,
		ndOffset,
		width,
		depth,
		fWidth,
		fDepth);

	nwOffset = wOffset;
	ndOffset = dOffset;

	// Bottom Left Box
	mChildren[2] = new QuadTree(
		XMVectorSet(c.x - e.x, c.y - e.y, c.z - e.z, 0),
		XMVectorSet(c.x, c.y + e.y, c.z, 0),
		mIndexCount,
		pIndices,
		nwOffset,
		ndOffset,
		width,
		depth,
		fWidth,
		fDepth);

	nwOffset = wOffset + width / 2;
	ndOffset = dOffset;

	// Bottom Right Box
	mChildren[3] = new QuadTree(
		XMVectorSet(c.x, c.y - e.y, c.z - e.z, 0),
		XMVectorSet(c.x + e.x, c.y + e.y, c.z, 0),
		mIndexCount,
		pIndices,
		nwOffset,
		ndOffset,
		width,
		depth,
		fWidth,
		fDepth);

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
	else
	{
		indexCount = 0;
		return false;
	}

	
}