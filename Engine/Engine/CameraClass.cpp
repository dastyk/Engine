#include "CameraClass.h"


CameraClass::CameraClass() : PositionClass()
{
	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mViewMatrix, I);
	XMStoreFloat4x4(&mProjMatrix, I);
	XMStoreFloat4x4(&mInvViewMatrix, I);
}


CameraClass::~CameraClass()
{
}


void CameraClass::CalcViewMatrix() 
{
	XMVECTOR position, lookAt, f, up;
	XMMATRIX rotationMatrix;

	// Calculate forward vector and up vector
	CalcForwardUpRightVector();

	// Setup the position of the camera in the world.
	position = XMLoadFloat3(&mPosition);

	f = XMLoadFloat3(&mForward);
	up = XMLoadFloat3(&mUpVector);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + f;

	// Finally create the view matrix from the three updated vectors.
	XMMATRIX viewMatrix = XMMatrixLookAtLH(position, lookAt, up);
	XMStoreFloat4x4(&mViewMatrix, viewMatrix);
	XMStoreFloat4x4(&mInvViewMatrix, XMMatrixInverse(nullptr, viewMatrix));

	return;
}


void CameraClass::SetProjMatrix(float FoV, float AspectRatio, float nearP, float farP)
{
	mFoV = FoV;
	mAspectRatio = AspectRatio;
	mNearPlane = nearP;
	mFarPlane = farP;

	XMMATRIX proj = XMMatrixPerspectiveFovLH(FoV, AspectRatio, nearP, farP);
	XMStoreFloat4x4(&mProjMatrix, proj);
	BoundingFrustum::CreateFromMatrix(mFrustum, proj);
}

void CameraClass::SetRotation(XMFLOAT3& rot)
{
	mRotation = rot;
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
}


XMFLOAT4X4 CameraClass::GetViewMatrix()const
{
	return (XMFLOAT4X4)mViewMatrix;
}

XMFLOAT4X4 CameraClass::GetProjMatrix()const
{
	return (XMFLOAT4X4)mProjMatrix;
}

BoundingFrustum CameraClass::GetBoundingFrustum()const
{
	XMMATRIX invView = XMLoadFloat4x4(&mInvViewMatrix);
	BoundingFrustum f;
	mFrustum.Transform(f, invView);
	return f;
}