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
	for (UINT i = 0; i < QUAD_TREE_CHILDREN_COUNT; i++)
	{
		if (mChildren[i])
		{
			delete mChildren[i];
			mChildren[i] = 0;
		}
		
	}
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

bool QuadTree::Init(XMVECTOR p1, XMVECTOR p2, QuadTree* pParent, UINT indexCount, UINT indexStart)
{
	bool result;

	XMFLOAT3 f1, f2;  
	XMStoreFloat3(&f1, p1);
	XMStoreFloat3(&f2, p2);

	BoundingBox::CreateFromPoints(mBox, p1, p2);
	mParent = pParent;
	mIndexCount = indexCount;
	mIndexStart = indexStart;

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
	UINT index = mIndexCount / 4;

	for (UINT i = 0; i < QUAD_TREE_CHILDREN_COUNT; i++)
	{
		mChildren[i] = new QuadTree;
	}

	// Top left box
	result = mChildren[0]->Init(
		XMVectorSet(c.x - e.x, c.y - e.y, c.z, 0),
		XMVectorSet(c.x, c.y + e.y, c.z + e.z, 0),
		this,
		index,
		mIndexStart + index * 2
		);
	if (!result)
		return false;

	// Top right box
	result = mChildren[1]->Init(
		XMVectorSet(c.x, c.y - e.y, c.z, 0),
		XMVectorSet(c.x + e.x, c.y + e.y, c.z + e.z, 0),
		this,
		index,
		mIndexStart + index * 3
		);
	if (!result)
		return false;

	// bottom left box
	result = mChildren[2]->Init(
		XMVectorSet(c.x - e.x, c.y - e.y, c.z - e.z, 0),
		XMVectorSet(c.x, c.y + e.y, c.z, 0),
		this,
		index,
		mIndexStart + index * 0
		);
	if (!result)
		return false;

	// bottom right box
	result = mChildren[3]->Init(
		XMVectorSet(c.x, c.y - e.y, c.z - e.z, 0),
		XMVectorSet(c.x + e.x, c.y + e.y, c.z, 0),
		this,
		index,
		mIndexStart + index * 1
		);
	if (!result)
		return false;

	return true;
}

int QuadTree::RenderAgainsQuadTree(ID3D11DeviceContext* pDeviceContext, TerrainShaderClass* pShader, ObjectClass* pObject, CameraClass* pCamera, PointLightClass* pLights, ID3D11ShaderResourceView* pShadowmap)
{
	//int result = 0;
	//int count = 0;
	//if (mChildren[0])
	//{
	//	for (UINT i = 0; i < QUAD_TREE_CHILDREN_COUNT; i++)
	//	{
	//		result = mChildren[i]->RenderAgainsQuadTree(pDeviceContext, pShader, pObject, pCamera, pLights, pShadowmap);
	//		count += result;
	//	}

	//	return count;
	//}
	//else
	//{
	//	pObject->SetAsObjectToBeDrawn(pDeviceContext, 0);
	//	result = pShader->RenderShadowsDeferred(
	//		pDeviceContext,
	//		pObject,
	//		pCamera,
	//		pLights,
	//		pShadowmap,
	//		mIndexCount,
	//		mIndexStart);

	//		return 1;
	//}

	int count = 0;
	BoundingFrustum f = pCamera->GetBoundingFrustum();
	int result = f.Contains(mBox);
	if (result == 0)
	{
		// Render nothing

		return 0;
	}
	else if (result == 2 || (!mChildren[0]))
	{
		// Render the whole thing
		pObject->SetAsObjectToBeDrawn(pDeviceContext, 0);
		result = pShader->RenderShadowsDeferred(
			pDeviceContext,
			pObject,
			pCamera,
			pLights,
			pShadowmap,
			mIndexCount,
			mIndexStart);
		if (!result)
			return -1;

		return 1;
		
	}
	else if (result == 1)
	{
		// check children
		//pObject->SetAsObjectToBeDrawn(pDeviceContext, 0);
		for (UINT i = 0; i < QUAD_TREE_CHILDREN_COUNT; i++)
		{
			result = mChildren[i]->RenderAgainsQuadTree(pDeviceContext, pShader, pObject, pCamera, pLights, pShadowmap);
			if (result == -1)
				return -1;
			count += result;
		}
		return count;
	}
	
}