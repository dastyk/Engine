#ifndef _MATERIALCLASS_H_
#define _MATERIALCLASS_H_

#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class MaterialClass
{
public:
	MaterialClass();
	MaterialClass(const MaterialClass* other);
	MaterialClass(XMFLOAT3& AmbientReflection, XMFLOAT3& DiffuseReflection, XMFLOAT3& SpecularReflection, float SpecularShinyPower);
	virtual ~MaterialClass();

public:
	void SetAmbientReflection(XMFLOAT3& AmbientReflection);
	void SetDiffuseReflection(XMFLOAT3& DiffuseReflection);
	void SetSpecularReflection(XMFLOAT3& SpecularReflection);
	void SetSpecularShinyPower(float SpecularShinyPower);

	XMFLOAT3 GetAmbientReflection()const;
	XMFLOAT3 GetDiffuseReflection()const;
	XMFLOAT3 GetSpecularReflection()const;
	float GetSpecularShinyPower()const;


protected:
	XMFLOAT3 mAmbientReflection;
	XMFLOAT3 mDiffuseReflection;
	XMFLOAT3 mSpecularReflection;
	float mSpecularShinyPower;
};

#endif