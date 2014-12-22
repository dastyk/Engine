#ifndef _OBJECTCLASS_H_
#define _OBJECTCLASS_H_

#pragma once

#include "ModelClass.h"
#include "MaterialClass.h"
#include "TransformationClass.h"

class ObjectClass
{
public:
	ObjectClass();
	ObjectClass(const ObjectClass& other);
	ObjectClass(ModelClass* pModel);
	ObjectClass(ModelClass* pModel, TransformationClass* pTransform);
	ObjectClass(ModelClass* pModel, TransformationClass* pTransform, MaterialClass* pMaterial);
	virtual ~ObjectClass();

public:
	void SetModel(ModelClass* pModel);

	void Update();
	void SetAsObjectToBeDrawn(ID3D11DeviceContext* pDeviceContext);
	bool SetAsObjectToBeDrawn(ID3D11DeviceContext*, BoundingFrustum& frustum);

	XMFLOAT4X4 GetWorldMatrix()const;
	int GetIndexCount()const;
	TransformationClass* GetTransformation()const;
	MaterialClass* GetMaterial()const;

	TextureClass* GetTexture()const;

protected:

	TransformationClass* mTransform;
	MaterialClass* mMaterial;
	ModelClass* mModel;
};

#endif