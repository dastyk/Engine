#include "QuadTree.h"


QuadTree::QuadTree()
{
	mObjects = 0;
	mObjectCount = 0;
	mIndexCount = 0;
	mIndexStart = 0;
	mParent = 0;
	for (UINT i = 0; i < QUAD_TREE_CHILDREN_COUNT; i++)
	{
		mChildren[i] = 0;
	}
}


QuadTree::~QuadTree()
{

}


bool QuadTree::Init(UINT pointCount, const XMFLOAT3* pPoints, size_t stride, UINT indexCount)
{
	bool result;
	BoundingBox::CreateFromPoints(mBox, pointCount, pPoints, stride);
	
	mParent = 0;
	mIndexCount = indexCount;
	mIndexStart = 0;

	result = createChildren();
	if (!result)
		return false;

	return true;
}

bool QuadTree::Init(XMVECTOR p1, XMVECTOR p2, QuadTree* pParent, UINT indexCount)
{
	bool result;
	BoundingBox::CreateFromPoints(mBox, p1, p2);
	mParent = pParent;
	mIndexCount = indexCount/4;

	if (!(mIndexCount <= QUAD_SIZE_MIN))
	{
		result = createChildren();
		if (!result)
			return false;
	}


	return true;
}

bool QuadTree::createChildren()
{
	bool result;
	XMFLOAT3 c = mBox.Center;
	XMFLOAT3 e = mBox.Extents;

	for (UINT i = 0; i < QUAD_TREE_CHILDREN_COUNT; i++)
	{
		mChildren[i] = new QuadTree;
	}

	// Top left box
	result = mChildren[0]->Init(
		XMVectorSet(c.x - e.x, c.y - e.y, c.z, 0),
		XMVectorSet(c.x, c.y + e.y, c.z + e.z, 0),
		this,
		mIndexCount
		);
	if (!result)
		return false;

	// Top right box
	result = mChildren[1]->Init(
		XMVectorSet(c.x, c.y - e.y, c.z, 0),
		XMVectorSet(c.x + e.x, c.y + e.y, c.z + e.y, 0),
		this,
		mIndexCount
		);
	if (!result)
		return false;

	// bottom left box
	result = mChildren[2]->Init(
		XMVectorSet(c.x - e.x, c.y - e.y, c.z - e.z, 0),
		XMVectorSet(c.x, c.y + e.y, c.z, 0),
		this,
		mIndexCount
		);
	if (!result)
		return false;

	// bottom right box
	result = mChildren[3]->Init(
		XMVectorSet(c.x, c.y - e.y, c.z - e.z, 0),
		XMVectorSet(c.x + e.x, c.y + e.y, c.z, 0),
		this,
		mIndexCount
		);
	if (!result)
		return false;


}