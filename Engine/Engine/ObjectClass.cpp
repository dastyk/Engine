#include "ObjectClass.h"


ObjectClass::ObjectClass()
{
	mModel = nullptr;
	mTransform = nullptr;
	mTime = 0;
}


ObjectClass::ObjectClass(const ObjectClass& other)
{
	this->mModel = other.mModel;
	this->mTransform = new TransformationClass(other.mTransform);
	this->mTime = new GameTimer();
	mTime->Reset();
	mTime->Start();
}

ObjectClass::ObjectClass(ModelClass* pModel)
{
	mModel = pModel;
	mTransform = new TransformationClass();
	this->mTime = new GameTimer();
	mTime->Reset();
	mTime->Start();
}

ObjectClass::ObjectClass(ModelClass* pModel, TransformationClass* pTransform)
{
	mModel = pModel;
	mTransform = pTransform;
	this->mTime = new GameTimer();
	mTime->Reset();
	mTime->Start();
}


ObjectClass::~ObjectClass()
{
	if (mTransform)
	{
		delete mTransform;
		mTransform = 0;
	}
	if (mTime)
	{
		delete mTime;
		mTime = 0;
	}
}


void ObjectClass::Update()
{
	mTime->Tick();
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