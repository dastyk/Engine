#include "ObjectClass.h"


ObjectClass::ObjectClass()
{
	mModel = nullptr;
	mTransform = nullptr;
	mMaterial = nullptr;
}


ObjectClass::ObjectClass(const ObjectClass& other)
{
	this->mModel = other.mModel;
	this->mTransform = new TransformationClass(other.mTransform);
	this->mMaterial = new MaterialClass(other.mMaterial);
}

ObjectClass::ObjectClass(ModelClass* pModel)
{
	mModel = pModel;
	mTransform = new TransformationClass();
	mMaterial = new MaterialClass();
}

ObjectClass::ObjectClass(ModelClass* pModel, TransformationClass* pTransform)
{
	mModel = pModel;
	mTransform = pTransform;
	mMaterial = new MaterialClass();
}

ObjectClass::ObjectClass(ModelClass* pModel, TransformationClass* pTransform, MaterialClass* pMaterial)
{
	mModel = pModel;
	mTransform = pTransform;
	mMaterial = pMaterial;
}

ObjectClass::~ObjectClass()
{
	if (mTransform)
	{
		delete mTransform;
		mTransform = 0;
	}
	if (mMaterial)
	{
		delete mMaterial;
		mMaterial = 0;
	}
}


void ObjectClass::Update()
{

}


void ObjectClass::SetAsObjectToBeDrawn(ID3D11DeviceContext* pDeviceContext)
{
	mModel->SetAsModelToBeDrawn(pDeviceContext);
}


bool ObjectClass::SetAsModelToBeDrawnFromViewFrustum(ID3D11DeviceContext* pDeviceContext, BoundingFrustum& frustum)
{
	if (mModel->SetAsModelToBeDrawnFromViewFrustum(pDeviceContext, frustum))
		return true;
	return false;
}

XMFLOAT4X4 ObjectClass::GetWorldMatrix() const
{
	return mTransform->GetWorldMatrix();
}

int ObjectClass::GetIndexCount()const
{
	return mModel->GetIndexCount();
}


TransformationClass* ObjectClass::GetTransformation()const
{
	return mTransform;
}

MaterialClass* ObjectClass::GetMaterial()const
{
	return mMaterial;
}

TextureClass* ObjectClass::GetTexture()const
{
	return mModel->GetTexture();
}

void ObjectClass::SetModel(ModelClass* pModel)
{
	mModel = pModel;
}
