#include "PositionClass.h"


PositionClass::PositionClass()
{
	mPosition = XMFLOAT3(0.0, 0.0, 0.0);
	mRotation = XMFLOAT3(0.0, 0.0, 0.0);
	mUpVector = XMVectorSet(0, 1, 0, 0);
	mForward = XMVectorSet(0.0, 0.0, 1.0, 0.0);
	mRightVector = XMVectorSet(1.0, 0.0, 0.0, 0.0);
	mMoveSpeed = 10;
	mLookSpeed = 5;
}

PositionClass::PositionClass(const PositionClass* other)
{
	this->mPosition = other->mPosition;
	this->mRotation = other->mRotation;
}

PositionClass::PositionClass(XMFLOAT3& Position, XMFLOAT3& Rotation)
{
	mPosition = Position;
	mRotation = Rotation;
}

PositionClass::~PositionClass()
{
}

void PositionClass::SetPosition(XMFLOAT3& pos)
{
	mPosition = pos;
}

void PositionClass::SetRotation(XMFLOAT3& rot)
{
	mRotation = rot;
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

XMFLOAT3 PositionClass::GetPosition()const
{
	return mPosition;
}

XMFLOAT3 PositionClass::GetRotation()const
{
	return mRotation;
}

void PositionClass::SetUpdateTime(float dt)
{
	mUpdateTime = dt;
}

void PositionClass::MoveForward(bool state)
{
	if (state)
	{
		XMVECTOR pos = XMLoadFloat3(&mPosition);
		pos += mForward*mUpdateTime*mMoveSpeed;
		XMStoreFloat3(&mPosition, pos);
	}
}

void PositionClass::MoveBackward(bool state)
{
	if (state)
	{
		XMVECTOR pos = XMLoadFloat3(&mPosition);
		pos -= mForward*mUpdateTime*mMoveSpeed;
		XMStoreFloat3(&mPosition, pos);
	}
}

void PositionClass::MoveUpward(bool state)
{
	if (state)
	{
		XMVECTOR pos = XMLoadFloat3(&mPosition);
		pos += mUpVector*mUpdateTime*mMoveSpeed;
		XMStoreFloat3(&mPosition, pos);
	}
}

void PositionClass::MoveDownward(bool state)
{
	if (state)
	{
		XMVECTOR pos = XMLoadFloat3(&mPosition);
		pos -= mUpVector*mUpdateTime*mMoveSpeed;
		XMStoreFloat3(&mPosition, pos);
	}
}

void PositionClass::MoveRight(bool state)
{
	if (state)
	{
		XMVECTOR pos = XMLoadFloat3(&mPosition);
		pos += mRightVector*mUpdateTime*mMoveSpeed;
		XMStoreFloat3(&mPosition, pos);
	}
}

void PositionClass::MoveLeft(bool state)
{
	if (state)
	{
		XMVECTOR pos = XMLoadFloat3(&mPosition);
		pos -= mRightVector*mUpdateTime*mMoveSpeed;
		XMStoreFloat3(&mPosition, pos);
	}
}


void PositionClass::TurnLeft(bool state)
{

}

void PositionClass::TurnRight(bool state)
{

}

void PositionClass::LookUpward(bool state)
{

}

void PositionClass::LookDownward(bool state)
{

}

void PositionClass::CalcForwardUpRightVector()
{
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	// Setup the vector that points upwards.
	mUpVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// Setup where the camera is looking by default.
	mForward = XMVectorSet(0.0, 0.0, 1.0, 0.0);

	yaw = XMConvertToRadians(mRotation.x);
	pitch = XMConvertToRadians(mRotation.y);
	roll = XMConvertToRadians(mRotation.z);

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);



	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	mForward = XMVector3TransformCoord(mForward, rotationMatrix);
	mUpVector = XMVector3TransformCoord(mUpVector, rotationMatrix);
	mRightVector = XMVector3Cross(mUpVector, mForward);
	
}