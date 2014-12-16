#ifndef _RAY_H_
#define _RAY_H_

#pragma once


#include <DirectXMath.h>

using namespace DirectX;

class Ray
{
public:
	Ray();
	virtual ~Ray();

	XMFLOAT3 GetStartPos()const;
	XMFLOAT3 GetDirection()const;

protected:
	XMFLOAT3 mStartPos;
	XMFLOAT3 mDirection;

};

#endif