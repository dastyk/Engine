#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#pragma once

#define QUAD_TREE_CHILDREN_COUNT 4
#define QUAD_SIZE_MIN 24576


#include "ObjectClass.h"
#include "TerrainShaderClass.h"
#include "DeferredShaderClass.h"

class QuadTree
{
public:
	QuadTree();	
	~QuadTree();

	bool Init(UINT pointCount, const XMFLOAT3* pPoints, size_t stride, UINT indexCount);


	int RenderAgainsQuadTree(ID3D11DeviceContext* pDeviceContext, TerrainShaderClass* pShader, DeferredShaderClass* pOShader, ObjectClass* pObject, CameraClass* pCamera, PointLightClass* pLights, ID3D11ShaderResourceView* pShadowmap);

	void AddModels(ObjectClass** ppObject, UINT nrOfObjects);
	bool AddModel(ObjectClass* pObject);
	void CopyFromVectorToArray();
private:
	bool createChildren();
	bool Init(XMVECTOR p1, XMVECTOR p2, QuadTree* pParent, UINT indexCount, UINT indexStart);
	
	bool AddModelHelper(ObjectClass* pObject);

	

private:
	vector<ObjectClass*> mTObjects;
	ObjectClass** mObjects;
	UINT mObjectCount;

	BoundingBox mBox;
	QuadTree* mChildren[QUAD_TREE_CHILDREN_COUNT];
	QuadTree* mParent;

	UINT mIndexStart;
	UINT mIndexCount;

};

#endif