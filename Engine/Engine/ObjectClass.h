#ifndef _OBJECTCLASS_H_
#define _OBJECTCLASS_H_

#pragma once

#include "ModelClass.h"
#include "TransformationClass.h"
#include "GameTimer.h"

class ObjectClass
{
public:
	ObjectClass();
	ObjectClass(const ObjectClass& other);
	ObjectClass(ModelClass* pModel);
	ObjectClass(ModelClass* pModel, TransformationClass* pTransform);
	virtual ~ObjectClass();

public:
	void SetModel(ModelClass* pModel);

	void Update();
	void SetAsObjectToBeDrawn(ID3D11DeviceContext* pDeviceContext, int flag);
	bool SetAsObjectToBeDrawn(ID3D11DeviceContext*, BoundingFrustum& frustum, int flag);

	XMFLOAT4X4 GetWorldMatrix()const;
	XMFLOAT4X4 GetNormalMatrix()const;
	int GetIndexCount()const;
	TransformationClass* GetTransformation()const;

	MatrialDesc* GetMaterials()const;
	ModelClass* GetModel()const;

	TextureClass* GetTexture()const;

	void Animate(XMFLOAT4X4** mBL);

protected:
	void interpolateFrames(Frame* f1, Frame* f2, XMFLOAT4X4* m);


protected:

	TransformationClass* mTransform;
	ModelClass* mModel;
	GameTimer* mTime;

	UINT mDCount;
	int mNrOfFrames;
};

#endif