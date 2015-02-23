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
	mSnow = 0;
}


QuadTree::~QuadTree()
{

	if (mObjects)
	{
		delete[] mObjects;
		mObjects = 0;
	}
	for (UINT i = 0; i < QUAD_TREE_CHILDREN_COUNT; i++)
	{
		if (mChildren[i])
		{
			delete mChildren[i];
			mChildren[i] = 0;
		}
		
	}
	if (mSnow)
	{
		delete mSnow;
		mSnow = 0;
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

int QuadTree::RenderAgainsQuadTree(ID3D11DeviceContext* pDeviceContext, TerrainShaderClass* pShader, DeferredShaderClass* pOShader, ObjectClass* pObject, CameraClass* pCamera, PointLightClass* pLights, ID3D11ShaderResourceView* pShadowmap)
{
	int count = 0;
	BoundingFrustum f = pCamera->GetBoundingFrustum();
	int result = f.Contains(mBox);
	if (result == 0)
	{
		// Render nothing

		return 0;
	}
	else if (result == 2)
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

		count++;
		

		for (UINT i = 0; i < mObjectCount; i++)
		{

			mObjects[i]->SetAsObjectToBeDrawn(pDeviceContext, 0);
			pOShader->RenderDeferred(pDeviceContext, mObjects[i], pCamera);
			count++;


		}



		return count;
		
	}
	else if ((!mChildren[0]))
	{
		// Render the whole thing and cull the objects
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

		count++;


		for (UINT i = 0; i < mObjectCount; i++)
		{

			if (mObjects[i]->SetAsObjectToBeDrawn(pDeviceContext, f, 0))
			{

				pOShader->RenderDeferred(pDeviceContext, mObjects[i], pCamera);
				count++;
			}

		}


		return count;
	}
	else if (result == 1)
	{
		// check children
		//pObject->SetAsObjectToBeDrawn(pDeviceContext, 0);
		for (UINT i = 0; i < QUAD_TREE_CHILDREN_COUNT; i++)
		{
			result = mChildren[i]->RenderAgainsQuadTree(pDeviceContext, pShader, pOShader, pObject, pCamera, pLights, pShadowmap);
			if (result == -1)
				return -1;
			count += result;
		}
		return count;
	}
	
}


int QuadTree::RenderLightsAgainsQuadTree(ID3D11DeviceContext* pDeviceContext, DeferredBufferClass* pBuffer, DeferredShaderClass* pShader, CameraClass* pCamera)
{
	int count = 0;
	BoundingFrustum f = pCamera->GetBoundingFrustum();
	int result = f.Contains(mBox);
	if (result == 0)
	{
		// Render nothing

		return 0;
	}
	else if (result == 2)
	{
		// Contains the whole thing
		UINT NR = mTLights.size();
		for (UINT i = 0; i < NR; i++)
		{
			pBuffer->ClearDepthBuffer(pDeviceContext);
			pShader->RenderLights(pDeviceContext, mTLights[i]);
		}


		return count;

	}
	else if ((!mChildren[0]))
	{
		// Leaf node
		BoundingSphere s;
		UINT NR = mTLights.size();
		for (UINT i = 0; i < NR; i++)
		{
			s = mTLights[i]->GetBoundingSphere();
			if (f.Intersects(s))
			{
				pBuffer->ClearDepthBuffer(pDeviceContext);
				pShader->RenderLights(pDeviceContext, mTLights[i]);
			}
		}
		return count;
	}
	else if (result == 1)
	{
		// check children
		//pObject->SetAsObjectToBeDrawn(pDeviceContext, 0);
		for (UINT i = 0; i < QUAD_TREE_CHILDREN_COUNT; i++)
		{
			result = mChildren[i]->RenderLightsAgainsQuadTree(pDeviceContext, pBuffer, pShader, pCamera);
			if (result == -1)
				return -1;
			count += result;
		}
		return count;
	}
}

void QuadTree::AddModels(ObjectClass** ppObject, UINT nrOfObjects)
{
	for (UINT i = 0; i < nrOfObjects; i++)
	{
		AddModel(ppObject[i]);
	}

	CopyFromVectorToArray();

}

bool QuadTree::AddModel(ObjectClass* pObject)
{
	BoundingBox b = pObject->GetBoundingBox();

	int result = mBox.Intersects(b);
	if (result)
	{
		mTObjects.push_back(pObject);
		AddModelHelper(pObject);
		return true;		
	}

	return false;
}

bool QuadTree::AddModelHelper(ObjectClass* pObject)
{
	
	if (mChildren[0])
	{
		for (UINT j = 0; j < QUAD_TREE_CHILDREN_COUNT; j++)
		{
			if (mChildren[j]->AddModel(pObject))
				return true;
		}
	}
	else
	{
		return false;
	}
}

void QuadTree::CopyFromVectorToArray()
{
	mObjectCount = mTObjects.size();
	mObjects = new ObjectClass*[mObjectCount];

	for (UINT i = 0; i < mObjectCount; i++)
	{
		mObjects[i] = mTObjects[i];
	}
	if (mChildren[0])
	{
		for (UINT i = 0; i < QUAD_TREE_CHILDREN_COUNT; i++)
		{
			mChildren[i]->CopyFromVectorToArray();
		}
	}

	//mTObjects.clear();
}


void QuadTree::AddLights(PointLightClass** ppPointLights, UINT nrOfLights)
{
	for (UINT i = 0; i < nrOfLights; i++)
	{
		AddLight(ppPointLights[i]);
	}

}

bool QuadTree::AddLight(PointLightClass* pPointLights)
{
	BoundingSphere b = pPointLights->GetBoundingSphere();

	int result = mBox.Intersects(b);
	if (result)
	{
		mTLights.push_back(pPointLights);
		AddLightHelper(pPointLights);
		return true;
	}

	return false;
}

bool QuadTree::AddLightHelper(PointLightClass* pPointLights)
{

	if (mChildren[0])
	{
		for (UINT j = 0; j < QUAD_TREE_CHILDREN_COUNT; j++)
		{
			if (mChildren[j]->AddLight(pPointLights))
				return true;
		}
	}
	else
	{
		return false;
	}
}

bool QuadTree::AddSnow(ID3D11Device* pDevice)
{
	bool result;
	if (mChildren[0])
	{
		for (UINT i = 0; i < QUAD_TREE_CHILDREN_COUNT; i++)
		{
			mChildren[i]->AddSnow(pDevice);
		}
	}
	else
	{
		mSnow = new SnowEffect();
		if (!mSnow)
			return false;

		result = mSnow->Init(pDevice, &mBox.Center, &mBox.Extents);
		if (!result)
			return false;
	}
}

void QuadTree::Update(float dt)
{
	if (mChildren[0])
	{
		for (UINT i = 0; i < QUAD_TREE_CHILDREN_COUNT; i++)
		{
			mChildren[i]->Update(dt);
		}
	}
	else
	{
		mSnow->Update(dt);
	}
}

bool QuadTree::RenderSnow(ID3D11DeviceContext* pDeviceContext, ParticleShaderClass* pPShader, CameraClass* pCamera)
{
	int cont = mBox.Contains(pCamera->GetBoundingBox());
	
	if (cont)
	{
		if (mChildren[0])
		{
			for (UINT i = 0; i < QUAD_TREE_CHILDREN_COUNT; i++)
			{
				mChildren[i]->RenderSnow(pDeviceContext, pPShader, pCamera);
			}
		}
		else
		{
			mSnow->render(pDeviceContext);
			pPShader->Render(pDeviceContext, mSnow, pCamera);

		}
	}
	return true;
}

bool QuadTree::SnowRenderHelper(ID3D11DeviceContext* pDeviceContext, ParticleShaderClass* pPShader, CameraClass* pCamera)
{
	bool result;
	if (!mChildren[0])
	{
		mSnow->render(pDeviceContext);
		result = pPShader->Render(pDeviceContext, mSnow, pCamera);
		if (!result)
			return false;
	}
	else
	{
		for (UINT i = 0; i < QUAD_TREE_CHILDREN_COUNT; i++)
		{
			result = mChildren[i]->SnowRenderHelper(pDeviceContext, pPShader, pCamera);
			if (!result)
				return false;
		}
	}

	return true;
}