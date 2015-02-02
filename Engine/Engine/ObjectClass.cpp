#include "ObjectClass.h"


ObjectClass::ObjectClass()
{
	mModel = nullptr;
	mTransform = nullptr;
}


ObjectClass::ObjectClass(const ObjectClass& other)
{
	this->mModel = other.mModel;
	this->mTransform = new TransformationClass(other.mTransform);
}

ObjectClass::ObjectClass(ModelClass* pModel)
{
	mModel = pModel;
	mTransform = new TransformationClass();
}

ObjectClass::ObjectClass(ModelClass* pModel, TransformationClass* pTransform)
{
	mModel = pModel;
	mTransform = pTransform;
}


ObjectClass::~ObjectClass()
{
	if (mTransform)
	{
		delete mTransform;
		mTransform = 0;
	}
}


void ObjectClass::Update()
{

}


void ObjectClass::SetAsObjectToBeDrawn(ID3D11DeviceContext* pDeviceContext)
{
	mModel->SetAsModelToBeDrawn(pDeviceContext);
}

bool ObjectClass::SetAsObjectToBeDrawn(ID3D11DeviceContext* pDeviceContext, BoundingFrustum& frustum)
{
	if (mModel->SetAsModelToBeDrawn(pDeviceContext, frustum))
		return true;
	else
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


TextureClass* ObjectClass::GetTexture()const
{
	return mModel->GetTexture();
}

void ObjectClass::SetModel(ModelClass* pModel)
{
	mModel = pModel;
}

MatrialDesc* ObjectClass::GetMaterials()const
{
	return mModel->GetMaterials();
}

ModelClass* ObjectClass::GetModel()const
{
	return mModel;
}