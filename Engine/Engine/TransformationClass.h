#ifndef _TRANSFORMATIONCLASS_H_
#define _TRANSFORMATIONCLASS_H_

#pragma once

#include <DirectXMath.h>

using namespace DirectX;

class TransformationClass
{
public:
	TransformationClass();
	TransformationClass(const TransformationClass* other);
	TransformationClass(XMFLOAT3& Position, XMFLOAT3& Rotation, XMFLOAT3& Scale);
	virtual ~TransformationClass();

public:
	void SetPosition(XMFLOAT3& Position);
	void SetRotation(XMFLOAT3& Rotation);
	void SetScale(XMFLOAT3& Scale);

	XMFLOAT3 GetPosition()const;
	XMFLOAT3 GetRotation()const;
	XMFLOAT3 GetScale()const;

	XMFLOAT4X4 GetWorldMatrix()const;

protected:

	XMFLOAT3 mPosition;
	XMFLOAT3 mRotation;
	XMFLOAT3 mScale;
};

#endif