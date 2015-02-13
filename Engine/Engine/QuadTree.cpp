#include "QuadTree.h"


QuadTree::QuadTree()
{
	mObjects = 0;
	mObjectCount = 0;
	mIndexBuffer = 0;
	for (UINT i = 0; i < QUAD_TREE_CHILDREN_COUNT; i++)
	{
		mChildren[i] = 0;
	}
}


QuadTree::~QuadTree()
{
	if (mIndexBuffer)
	{
		mIndexBuffer->Release();
		mIndexBuffer = 0;
	}
}


bool QuadTree::Init(ID3D11Device* pDevice, int pointCount, const XMFLOAT3* pPoints, size_t stride, int indexCount, unsigned long* pIndices, int width, int depth)
{
	BoundingBox::CreateFromPoints(mBox, pointCount, pPoints, stride);

}


bool QuadTree::createChildren()
{

}