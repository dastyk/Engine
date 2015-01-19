#ifndef _PARTICLECLASS_H_
#define _PARTICLECLASS_H_
#pragma once

#include "TransformationClass.h"
#include "GameTimer.h"

class ParticleClass
{
public:
	ParticleClass();
	ParticleClass(XMFLOAT3& Position, XMFLOAT3& Color, XMFLOAT3& travelDir, float moveSpeed, int mMaxLifeTime);
	ParticleClass(const ParticleClass& other);
	ParticleClass(const ParticleClass* other);
	ParticleClass(const ParticleClass* other, XMFLOAT3& travelDir, float moveSpeed, int lifeTime);
	~ParticleClass();

	void Update(float dt);


	XMFLOAT3 GetColor()const;
	TransformationClass* GetTranform()const;

	bool isAlive()const;

private:
	TransformationClass* mTransform;
	XMFLOAT3 mColor, mTravelDirection;
	int mMaxLifeTime;
	GameTimer* mLifeTime;
};

#endif