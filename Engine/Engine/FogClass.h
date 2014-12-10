#ifndef _FOGCLASS_H_
#define _FOGCLASS_H_


#pragma once


#include <DirectXMath.h>
using namespace DirectX;

class FogClass
{
public:
	FogClass();
	FogClass(XMFLOAT3& pos, XMFLOAT3& color, float range);
	virtual ~FogClass();

	void SetPosition(XMFLOAT3& pos);
	void SetColor(XMFLOAT3& color);
	void SetRange(float& range);

	XMFLOAT3 GetPosition()const;
	XMFLOAT3 GetColor()const;
	float GetRange()const;

protected:
	XMFLOAT3 mPosition;
	XMFLOAT3 mColor;
	float mRange;

};

#endif