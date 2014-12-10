#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_

#pragma once

#include <DirectXMath.h>
using namespace DirectX;


class CameraClass
{
public:
	CameraClass();
	~CameraClass();

	void SetPosition(XMFLOAT3&);
	void SetRotation(XMFLOAT3&);
	void SetProjMatrix(float FoV, float AspectRatio, float nearP, float farP);
	void moveForward();

	XMFLOAT3 GetPosition() const;
	XMFLOAT3 GetRotation() const;
	XMFLOAT4X4 GetViewMatrix()const;
	XMFLOAT4X4 GetProjMatrix()const;


	void CalViewMatrix();
	

private:
	XMFLOAT3 mPosition;
	XMFLOAT3 mRotation;
	XMFLOAT4X4 mViewMatrix;
	XMFLOAT4X4 mProjMatrix;
	XMFLOAT4X4 mOrthoMatix;
};

#endif