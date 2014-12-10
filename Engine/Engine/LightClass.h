#ifndef _LIGHTCLASS_H_
#define _LIGHTCLASS_H_

#pragma once

#include <DirectXMath.h>
using namespace DirectX;


class LightClass
{
public:
	LightClass();
	LightClass(XMFLOAT3& dir, XMFLOAT3& color, XMFLOAT3& pos);
	virtual ~LightClass();

	void SetLightDir(XMFLOAT3&);
	void SetLightColor(XMFLOAT3&);
	void SetLightPos(XMFLOAT3&);

	XMFLOAT3 GetLightDir()const;
	XMFLOAT3 GetLightColor()const;
	XMFLOAT3 GetLightPos()const;

protected:
	XMFLOAT3 mLightDir;
	XMFLOAT3 mLightColor;
	XMFLOAT3 mLightPos;

};

#endif