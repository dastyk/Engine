#ifndef _TRANSFORMATIONCLASS_H_
#define _TRANSFORMATIONCLASS_H_

#pragma once
#include "PositionClass.h"


class TransformationClass : public PositionClass
{
public:
	TransformationClass();
	TransformationClass(const TransformationClass* other);
	TransformationClass(XMFLOAT3& Position, XMFLOAT3& Rotation, XMFLOAT3& Scale);
	virtual ~TransformationClass();

public:
	void SetScale(XMFLOAT3& Scale);

	XMFLOAT3 GetScale()const;

	XMFLOAT4X4 GetWorldMatrix()const;

protected:

	XMFLOAT3 mScale;
};

#endif