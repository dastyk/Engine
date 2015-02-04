#include "PositionClass.h"


PositionClass::PositionClass()
{
	mPosition = XMFLOAT3(0.0, 0.0, 0.0);
	mRotation = XMFLOAT3(0.0, 0.0, 0.0);
	mUpVector = XMFLOAT3(0.0, 1.0, 0.0);
	mForward = XMFLOAT3(0.0, 0.0, 1.0);
	mRightVector = XMFLOAT3(1.0, 0.0, 0.0);
	mMoveSpeed = 50;
	mLookSpeed = 5;
	mLastPosIndex = 0;
	for (int i = 0; i < SAMPLES_POSITION; i++)
	{
		mLastPos[i] = mPosition;
	}
}

PositionClass::PositionClass(const PositionClass* other)
{
	this->mPosition = other->mPosition;
	this->mRotation = other->mRotation;
}

PositionClass::PositionClass(const XMFLOAT3& Position, const XMFLOAT3& Rotation)
{
	mPosition = Position;
	mRotation = Rotation;
	mUpVector = XMFLOAT3(0.0, 1.0, 0.0);
	mForward = XMFLOAT3(0.0, 0.0, 1.0);
	mRightVector = XMFLOAT3(1.0, 0.0, 0.0);
	mMoveSpeed = 1;
	mLookSpeed = 5;
}
PositionClass::PositionClass(const XMFLOAT3& Position, const XMFLOAT3& Rotation, const float moveSpeed)
{
	mPosition = Position;
	mRotation = Rotation;
	mUpVector = XMFLOAT3(0.0, 1.0, 0.0);
	mForward = XMFLOAT3(0.0, 0.0, 1.0);
	mRightVector = XMFLOAT3(1.0, 0.0, 0.0);
	mMoveSpeed = moveSpeed;
	mLookSpeed = 5;
}

PositionClass::~PositionClass()
{
}

void PositionClass::SetPosition(const XMFLOAT3& pos)
{
	//mLastPos[mLastPosIndex] = mPosition;
	mPosition = pos;
	//mLastPosIndex = (mLastPosIndex + 1) % SAMPLES_POSITION;
}

void PositionClass::SetPosition(const XMVECTOR& pos)
{
	//mLastPos[mLastPosIndex] = mPosition;
	XMStoreFloat3(&mPosition, pos);
	//mLastPosIndex = (mLastPosIndex + 1) % SAMPLES_POSITION;
}

void PositionClass::SetMoveSpeed(const float speed)
{
	mMoveSpeed = speed;
}

void PositionClass::SetRotation(const XMFLOAT3& rot)
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

void PositionClass::SetUpdateTime(const float dt)
{
	mUpdateTime = dt;
}

void PositionClass::MoveForward(bool state)
{
	if (state)
	{
		XMVECTOR pos = XMLoadFloat3(&mPosition);
		XMVECTOR f = XMLoadFloat3(&mForward);
		pos += f*mUpdateTime*mMoveSpeed;
		SetPosition(pos);
	}
}

void PositionClass::MoveBackward(bool state)
{
	if (state)
	{
		XMVECTOR pos = XMLoadFloat3(&mPosition);
		XMVECTOR f = XMLoadFloat3(&mForward);
		pos -= f*mUpdateTime*mMoveSpeed;
		SetPosition(pos);
	}
}

void PositionClass::MoveUpward(bool state)
{
	if (state)
	{
		XMVECTOR pos = XMLoadFloat3(&mPosition);
		XMVECTOR up = XMLoadFloat3(&mUpVector);
		pos += up*mUpdateTime*mMoveSpeed;
		SetPosition(pos);
	}
}

void PositionClass::MoveDownward(bool state)
{
	if (state)
	{
		XMVECTOR pos = XMLoadFloat3(&mPosition);
		XMVECTOR up = XMLoadFloat3(&mUpVector);
		pos -= up*mUpdateTime*mMoveSpeed;
		SetPosition(pos);
	}
}

void PositionClass::MoveRight(bool state)
{
	if (state)
	{
		XMVECTOR pos = XMLoadFloat3(&mPosition);
		XMVECTOR r = XMLoadFloat3(&mRightVector);
		pos += r*mUpdateTime*mMoveSpeed;
		SetPosition(pos);
	}
}

void PositionClass::MoveLeft(bool state)
{
	if (state)
	{
		XMVECTOR pos = XMLoadFloat3(&mPosition);
		XMVECTOR r = XMLoadFloat3(&mRightVector);
		pos -= r*mUpdateTime*mMoveSpeed;
		SetPosition(pos);
	}
}

void PositionClass::MoveAlongVector(const XMFLOAT3& dir)
{
	XMVECTOR pos = XMLoadFloat3(&mPosition);
	XMVECTOR v = XMLoadFloat3(&dir);

	pos += v*mUpdateTime*mMoveSpeed;
	SetPosition(pos);
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
	mUpVector = XMFLOAT3(0.0f, 1.0f, 0.0f);

	// Setup where the camera is looking by default.
	mForward = XMFLOAT3(0.0, 0.0, 1.0);

	XMVECTOR forward = XMLoadFloat3(&mForward);
	XMVECTOR up = XMLoadFloat3(&mUpVector);

	yaw = XMConvertToRadians(mRotation.x);
	pitch = XMConvertToRadians(mRotation.y);
	roll = XMConvertToRadians(mRotation.z);

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	forward = XMVector3TransformCoord(forward, rotationMatrix);
	up = XMVector3TransformCoord(up, rotationMatrix);
	XMVECTOR right = XMVector3Cross(up, forward);

	XMStoreFloat3(&mUpVector, up);
	XMStoreFloat3(&mForward, forward);
	XMStoreFloat3(&mRightVector, right);
}

float PositionClass::GetAvgPosY()const
{
	float avg = 0;
	int count = 0;
	for (int i = 0; i < SAMPLES_POSITION; i++)
	{
		avg += mLastPos[i].y;
	}
	
	return avg / SAMPLES_POSITION;
}