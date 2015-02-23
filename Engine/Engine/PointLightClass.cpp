#include "PointLightClass.h"


PointLightClass::PointLightClass()
{
}
PointLightClass::PointLightClass(XMFLOAT3 color, XMFLOAT3 pos, float Radius) : LightClass(XMFLOAT3(0, 0, 0), color, pos)
{
	mRadius = Radius;
	mSphere.Center = pos;
	mSphere.Radius = Radius;
}

PointLightClass::~PointLightClass()
{
}

float PointLightClass::GetRadius()
{
	return mRadius;
}

void PointLightClass::SetRadius(float radius)
{
	mRadius = radius;
}

void PointLightClass::CalcViewMatrix()
{
	XMVECTOR position, lookAt, f, up;
	XMMATRIX rotationMatrix;

	// Setup the position of the camera in the world.
	position = XMLoadFloat3(&mLightPos);

	f = XMLoadFloat3(&mLightDir);
	up = XMVectorSet(0, 1, 0, 0);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + f;

	// Finally create the view matrix from the three updated vectors.
	XMMATRIX viewMatrix = XMMatrixLookAtLH(position, lookAt, up);
	XMStoreFloat4x4(&mViewMatrix, viewMatrix);
	XMStoreFloat4x4(&mInvViewMatrix, XMMatrixInverse(nullptr, viewMatrix));

	return;
}

void PointLightClass::SetProjMatrix(float FoV, float AspectRatio, float nearP, float farP)
{
	XMMATRIX proj = XMMatrixPerspectiveFovLH(FoV, AspectRatio, nearP, farP);
	XMStoreFloat4x4(&mProjMatrix, proj);

	BoundingFrustum::CreateFromMatrix(mFrustum, proj);
}

XMFLOAT4X4 PointLightClass::GetViewMatrix()const
{
	return (XMFLOAT4X4)mViewMatrix;
}

XMFLOAT4X4 PointLightClass::GetProjMatrix()const
{
	return (XMFLOAT4X4)mProjMatrix;
}

BoundingFrustum PointLightClass::GetBoundingFrustum()const
{
	XMMATRIX invView = XMLoadFloat4x4(&mInvViewMatrix);
	BoundingFrustum f;
	mFrustum.Transform(f, invView);
	return f;
}

BoundingSphere PointLightClass::GetBoundingSphere()const
{
	return mSphere;
}

void PointLightClass::SetLightPos(XMFLOAT3& pos)
{
	mLightPos = pos;
	mSphere.Center = pos;
}