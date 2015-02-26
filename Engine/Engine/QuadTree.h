#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#pragma once

#define QUAD_TREE_CHILDREN_COUNT 4
#define QUAD_SIZE_MIN 24576


#include "ObjectClass.h"
#include "TerrainShaderClass.h"
#include "DeferredShaderClass.h"
#include "SnowEffect.h"
#include "ParticleShaderClass.h"
class QuadTree
{
public:
	QuadTree();	
	~QuadTree();

	bool Init(UINT pointCount, const XMFLOAT3* pPoints, size_t stride, UINT* indexCount, UINT nrOfDetailLevels);


	int RenderAgainsQuadTree(ID3D11DeviceContext* pDeviceContext, TerrainShaderClass* pShader, DeferredShaderClass* pOShader, ObjectClass* pObject, CameraClass* pCamera, PointLightClass* pLights, ID3D11ShaderResourceView* pShadowmap);
	int RenderLightsAgainsQuadTree(ID3D11DeviceContext* pDeviceContext,DeferredBufferClass* pBuffer, DeferredShaderClass* pShader, CameraClass* pCamera);

	void AddModels(ObjectClass** ppObject, UINT nrOfObjects);
	void AddLights(PointLightClass** ppPointLights, UINT nrOfLights);

	bool AddModel(ObjectClass* pObject);
	bool AddLight(PointLightClass* pPointLights);
	bool AddSnow(ID3D11Device* pDevice);
	bool RenderSnow(ID3D11DeviceContext* pDeviceContext, ParticleShaderClass* pPShader, CameraClass* pCamera);
	bool SnowRenderHelper(ID3D11DeviceContext* pDeviceContext, ParticleShaderClass* pPShader, CameraClass* pCamera);

	void CopyFromVectorToArray();

	void Update(float dt, CameraClass* pCamera);




private:
	bool createChildren();
	bool Init(XMVECTOR p1, XMVECTOR p2, QuadTree* pParent, UINT* indexCount, UINT nrOfDetailLevels, UINT* indexStart, UINT id);
	
	bool AddModelHelper(ObjectClass* pObject);
	bool AddLightHelper(PointLightClass* pPointLights);
	
	UINT GetTerrainDetail(XMFLOAT3* p);

private:
	vector<ObjectClass*> mTObjects;
	vector<PointLightClass*> mTLights;
	ObjectClass** mObjects;
	UINT mObjectCount;

	BoundingBox mBox;
	QuadTree* mChildren[QUAD_TREE_CHILDREN_COUNT];
	QuadTree* mParent;

	UINT* mIndexStart;
	UINT* mIndexCount;
	UINT mNrOfDetailLevels;

	SnowEffect* mSnow;

};

#endif