#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_
#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "TextureClass.h"
#include <DirectXCollision.h>
#include "OBJ_Loader.h"
#include "AABB.h"

using namespace DirectX;

struct BoundingVertex
{
	XMFLOAT3 Pos;
};

class ModelClass
{
public:
	ModelClass();
	ModelClass(const ModelClass&);
	virtual ~ModelClass();

	

	void SetAsModelToBeDrawn(ID3D11DeviceContext*, UINT detaiLevel);
	virtual bool SetAsModelToBeDrawn(ID3D11DeviceContext*, BoundingFrustum& frustum, UINT detaiLevel);
	BoundingBox GetBoundingBox()const;

	virtual UINT GetIndexCount(UINT detailLevel)const;

	TextureClass* GetTexture()const;
	TextureClass* GetNormalMap()const;
	TextureClass* GetDetailMap()const;

	MatrialDesc* GetMaterials()const;

	SubsetTableDesc* GetSubSetTable()const;

	UINT GetObjectCount()const;
	UINT GetMaterialCount()const;
	UINT GetBoneCount()const;
	Bone* GetBones()const;
	UINT GetAnimationClipCount()const;
	AnimClipRead* GetAnimationClips()const;

	bool createModel(ID3D11Device*,char* modelName);

	

protected:
	bool createVertexBuffer(ID3D11Device*, D3D11_SUBRESOURCE_DATA*, UINT);
	bool createVertexBuffer(ID3D11Device* pDevice, ID3D11Buffer **ppBuffer, UINT byteWidth);
	bool createVertexBuffer(ID3D11Device*, D3D11_SUBRESOURCE_DATA*, ID3D11Buffer **ppBuffer, UINT);
	bool createIndexBuffer(ID3D11Device*, D3D11_SUBRESOURCE_DATA*, UINT);
	bool createIndexBuffer(ID3D11Device*, ID3D11Buffer**, UINT);
	bool createIndexBuffer(ID3D11Device* pDevice, D3D11_SUBRESOURCE_DATA* pData, ID3D11Buffer **ppBuffer, UINT byteWidth);


	TextureClass *mTexture;
	TextureClass* mNormalMap;
	TextureClass* mDetailMap;

	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer** mIndexBuffer;
	UINT mVertexCount, mObjectCount, mBoneCount, mAnimationClipCount, mMaterialCount;
	UINT* mIndexCount;
	UINT mNrOfDetailLevels;
	UINT mStride;
	MatrialDesc* mMaterial;
	Bone* mBones;
	AnimClipRead* mAnimationClips;
	SubsetTableDesc* mSubsetTable;

	AABB* mBox;
	ID3D11Buffer* mBoundingBoxVBuffer, *mBoundingBoxIBuffer;
};

#endif