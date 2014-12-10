#include "CameraClass.h"


CameraClass::CameraClass()
{
	mPosition.x = 0.0f;
	mPosition.y = 0.0f;
	mPosition.z = 0.0f;

	mRotation.x = 0.0f;
	mRotation.y = 0.0f;
	mRotation.z = 0.0f;

	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mViewMatrix, I);
	XMStoreFloat4x4(&mProjMatrix, I);
	XMStoreFloat4x4(&mOrthoMatix, I);
}




CameraClass::~CameraClass()
{
}

void CameraClass::SetPosition(XMFLOAT3& position)
{
	mPosition = position;
	return;
}


void CameraClass::SetRotation(XMFLOAT3& rotation)
{
	mRotation = rotation;
	if (mRotation.x > 360)
		mRotation.x = 0;
	if (mRotation.y > 90)
		mRotation.y = 90;
	if (mRotation.z > 360)
		mRotation.z = 0;
	if (mRotation.x < -360)
		mRotation.x = 0;
	if (mRotation.y < -90)
		mRotation.y = -90;
	if (mRotation.z < -360)
		mRotation.z = 0;
	return;
}

void CameraClass::moveForward()
{
	XMVECTOR lookAt, pos;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	// Setup where the camera is looking by default.
	lookAt = XMVectorSet(0.0, 0.0, 1.0, 0.0);

	yaw = XMConvertToRadians(mRotation.x);
	pitch = XMConvertToRadians(mRotation.y);
	roll = XMConvertToRadians(mRotation.z);

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);

	pos = XMLoadFloat3(&mPosition);

	pos = pos + lookAt*10;

	XMStoreFloat3(&mPosition, pos);
}

void CameraClass::CalViewMatrix()
{
	XMVECTOR up, position, lookAt;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;


	// Setup the vector that points upwards.
	up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f);

	// Setup the position of the camera in the world.
	position = XMLoadFloat3(&mPosition);

	// Setup where the camera is looking by default.
	lookAt = XMVectorSet(0.0, 0.0, 1.0, 0.0);

	yaw = XMConvertToRadians(mRotation.x);
	pitch = XMConvertToRadians(mRotation.y);
	roll = XMConvertToRadians(mRotation.z);

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);
	up = XMVector3TransformCoord(up, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	XMMATRIX viewMatrix = XMMatrixLookAtLH(position, lookAt, up);
	XMStoreFloat4x4(&mViewMatrix, viewMatrix);
	return;
}


void CameraClass::SetProjMatrix(float FoV, float AspectRatio, float nearP, float farP)
{
	XMMATRIX proj = XMMatrixPerspectiveFovLH(FoV, AspectRatio, nearP, farP);
	XMStoreFloat4x4(&mProjMatrix, proj);
}


XMFLOAT3 CameraClass::GetPosition()const
{
	return (XMFLOAT3)mPosition;
}


XMFLOAT3 CameraClass::GetRotation()const
{
	return (XMFLOAT3)mRotation;
}

XMFLOAT4X4 CameraClass::GetViewMatrix()const
{
	return (XMFLOAT4X4)mViewMatrix;
}

XMFLOAT4X4 CameraClass::GetProjMatrix()const
{
	return (XMFLOAT4X4)mProjMatrix;
}