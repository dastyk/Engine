#include "TransformationClass.h"


TransformationClass::TransformationClass()
{
	mPosition = XMFLOAT3(0.0, 0.0, 0.0);
	mRotation = XMFLOAT3(0.0, 0.0, 0.0);
	mScale = XMFLOAT3(1.0, 1.0, 1.0);
}

TransformationClass::TransformationClass(const TransformationClass* other)
{
	this->mPosition = other->mPosition;
	this->mRotation = other->mRotation;
	this->mScale = other->mScale;
}

TransformationClass::TransformationClass(XMFLOAT3& Position, XMFLOAT3& Rotation, XMFLOAT3& Scale)
{
	mPosition = Position;
	mRotation = Rotation;
	mScale = Scale;
}


TransformationClass::~TransformationClass()
{
}

XMFLOAT4X4 TransformationClass::GetWorldMatrix() const
{
	XMMATRIX world = XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);
	XMMATRIX rotX = XMMatrixRotationX(XMConvertToRadians(mRotation.x));
	XMMATRIX rotY = XMMatrixRotationY(XMConvertToRadians(mRotation.y));
	XMMATRIX rotZ = XMMatrixRotationZ(XMConvertToRadians(mRotation.z));
	XMMATRIX scale = XMMatrixScaling(mScale.x, mScale.y, mScale.z);

	XMMATRIX transform = scale*rotX*rotY*rotZ*world;

	XMFLOAT4X4 fTransform;
	XMStoreFloat4x4(&fTransform, transform);

	return fTransform;

}

XMFLOAT3 TransformationClass::GetPosition()const
{
	return mPosition;
}

XMFLOAT3 TransformationClass::GetRotation()const
{
	return mRotation;
}

XMFLOAT3 TransformationClass::GetScale()const
{
	return mScale;
}

void TransformationClass::SetPosition(XMFLOAT3& Position)
{
	mPosition = Position;
}

void TransformationClass::SetRotation(XMFLOAT3& Rotation)
{
	mRotation = Rotation;
	if (mRotation.x > 360)
		mRotation.x = 0;
	if (mRotation.y > 360)
		mRotation.y = 0;
	if (mRotation.z > 360)
		mRotation.z = 0;
	if (mRotation.x < -360)
		mRotation.x = 0;
	if (mRotation.y < -360)
		mRotation.y = 0;
	if (mRotation.z < -360)
		mRotation.z = 0;
}

void TransformationClass::SetScale(XMFLOAT3& Scale)
{
	mScale = Scale;
}