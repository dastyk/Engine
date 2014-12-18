#include "QuadTree.h"


QuadTree::QuadTree(int pointCount, const XMFLOAT3* pPoints, float minSize)
{
	AABB::createFromPoints(pointCount, pPoints);

	QuadTree::createChildren(minSize);
}

QuadTree::~QuadTree()
{
	for (int i = 0; i < 4; i++)
	{
		delete mChildren[i];
	}

}

void QuadTree::createChildren(float minSize)
{
	XMFLOAT3 c = mBox.Center;
	XMFLOAT3 e = mBox.Extents;

	if (e.x < minSize || e.y < minSize || e.z < minSize)
	{
		for (int i = 0; i < 4; i++)
			mChildren[i] = nullptr;
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


		XMFLOAT3 b0[8] = { p1, p2, p3, p4, p9, p10, p11, p12 };
		mChildren[0] = new QuadTree(8, b0);

		XMFLOAT3 b1[8] = { p1, p2, p3, p4, p5, p6, p13, p14 };
		mChildren[1] = new QuadTree(8, b1);

		XMFLOAT3 b2[8] = { p1, p2, p7, p8, p9, p10, p15, p16 };
		mChildren[2] = new QuadTree(8, b2);

		XMFLOAT3 b3[8] = { p1, p2, p7, p8, p5, p6, p17, p18 };
		mChildren[3] = new QuadTree(8, b3);
	}
}