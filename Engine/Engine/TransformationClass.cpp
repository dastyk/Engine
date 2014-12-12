#include "TransformationClass.h"


TransformationClass::TransformationClass() : PositionClass()
{
	mScale = XMFLOAT3(1.0, 1.0, 1.0);
}

TransformationClass::TransformationClass(const TransformationClass* other) : PositionClass(other)
{
	this->mScale = other->mScale;
}

TransformationClass::TransformationClass(XMFLOAT3& Position, XMFLOAT3& Rotation, XMFLOAT3& Scale) : PositionClass(Position,Rotation)
{
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

XMFLOAT3 TransformationClass::GetScale()const
{
	return mScale;
}

void TransformationClass::SetScale(XMFLOAT3& Scale)
{
	mScale = Scale;
}