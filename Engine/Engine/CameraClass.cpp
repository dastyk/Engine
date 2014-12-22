#include "CameraClass.h"


CameraClass::CameraClass() : PositionClass()
{
	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mViewMatrix, I);
	XMStoreFloat4x4(&mProjMatrix, I);
}


CameraClass::~CameraClass()
{
}


void CameraClass::CalcViewMatrix() 
{
	XMVECTOR position, lookAt;
	XMMATRIX rotationMatrix;

	// Calculate forward vector and up vector
	CalcForwardUpRightVector();

	// Setup the position of the camera in the world.
	position = XMLoadFloat3(&mPosition);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + mForward;

	// Finally create the view matrix from the three updated vectors.
	XMMATRIX viewMatrix = XMMatrixLookAtLH(position, lookAt, mUpVector);
	XMStoreFloat4x4(&mViewMatrix, viewMatrix);

	XMMATRIX proj = XMMatrixPerspectiveFovLH(mFoV, mAspectRatio, mNearPlane, mFarPlane);
	BoundingFrustum::CreateFromMatrix(mFrustrum, proj);
	XMVECTOR det;
	XMMATRIX invViewMatrix = XMMatrixInverse(&det, viewMatrix);
	mFrustrum.Transform(mFrustrum, invViewMatrix);

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
	BoundingFrustum::CreateFromMatrix(mFrustrum, proj);
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

BoundingFrustum CameraClass::GetBoundingFrustrum()const
{
	return mFrustrum;
}