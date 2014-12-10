#include "MaterialClass.h"


MaterialClass::MaterialClass()
{
	mAmbientReflection = XMFLOAT3(1, 1, 1);
	mDiffuseReflection = XMFLOAT3(1, 1, 1);
	mSpecularReflection = XMFLOAT3(1, 1, 1);
	mSpecularShinyPower = 250;
}

MaterialClass::MaterialClass(const MaterialClass* other)
{
	this->mAmbientReflection = other->mAmbientReflection;
	this->mDiffuseReflection = other->mDiffuseReflection;
	this->mSpecularReflection = other->mSpecularReflection;
	this->mSpecularShinyPower = other->mSpecularShinyPower;
}

MaterialClass::MaterialClass(XMFLOAT3& AmbientReflection, XMFLOAT3& DiffuseReflection, XMFLOAT3& SpecularReflection, float SpecularShinyPower)
{
	mAmbientReflection = AmbientReflection;
	mDiffuseReflection = DiffuseReflection;
	mSpecularReflection = SpecularReflection;
	mSpecularShinyPower = SpecularShinyPower;
}

MaterialClass::~MaterialClass()
{
}

XMFLOAT3 MaterialClass::GetAmbientReflection()const
{
	return mAmbientReflection;
}

XMFLOAT3 MaterialClass::GetDiffuseReflection()const
{
	return mDiffuseReflection;
}

XMFLOAT3 MaterialClass::GetSpecularReflection()const
{
	return mSpecularReflection;
}

float MaterialClass::GetSpecularShinyPower()const
{
	return mSpecularShinyPower;
}

void MaterialClass::SetAmbientReflection(XMFLOAT3& AmbientReflection)
{
	mAmbientReflection = AmbientReflection;
}

void MaterialClass::SetDiffuseReflection(XMFLOAT3& DiffuseReflection)
{
	mDiffuseReflection = DiffuseReflection;
}

void MaterialClass::SetSpecularReflection(XMFLOAT3& SpecularReflection)
{
	mSpecularReflection = SpecularReflection;
}

void MaterialClass::SetSpecularShinyPower(float SpecularShinyPower)
{
	mSpecularShinyPower = SpecularShinyPower;
}