#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_
#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "TextureClass.h"
#include <DirectXCollision.h>
#include "OBJ_Loader.h"

using namespace DirectX;


class ModelClass
{
public:
	ModelClass();
	ModelClass(const ModelClass&);
	virtual ~ModelClass();

	

	void SetAsModelToBeDrawn(ID3D11DeviceContext*);
	virtual bool SetAsModelToBeDrawn(ID3D11DeviceContext*, BoundingFrustum& frustum);


	virtual UINT GetIndexCount()const;

	TextureClass* GetTexture()const;
	MatrialDesc* GetMaterials()const;

	UINT GetObjectCount()const;

	bool createModel(ID3D11Device*,char* modelName);

protected:
	bool createVertexBuffer(ID3D11Device*, D3D11_SUBRESOURCE_DATA*, UINT);
	bool createVertexBuffer(ID3D11Device* pDevice, ID3D11Buffer **ppBuffer, UINT byteWidth);
	bool createIndexBuffer(ID3D11Device*, D3D11_SUBRESOURCE_DATA*, UINT);
	bool createIndexBuffer(ID3D11Device*, ID3D11Buffer**, UINT);

protected:
	TextureClass *mTexture;
	ID3D11Buffer* mVertexBuffer, *mIndexBuffer;
	UINT mVertexCount, mIndexCount, mObjectCount, mBoneCount, mAnimationClipCount;
	UINT mStride;
	MatrialDesc* mMaterial;
	Bone* mBones;
	AnimClipRead* mAnimationClips;
};

#endif