#ifndef _SNOWEFFECT_H_
#define _SNOWEFFECT_H_

#pragma once


#include "ParticleSystemClass.h"
class SnowEffect :
	public ParticleSystemClass
{
public:
	SnowEffect();
	~SnowEffect();

	bool Init(ID3D11Device*);
	void Update(float dt);

	void SetPlayerPos(XMFLOAT3& position);

private:
	void createFirstParticles();

	void CreateConstantInUpdate(ParticleClass*);
	void CreateEmitterInUpdate(ParticleClass*);
	void CreateMovingInUpdate(ParticleClass*);

	XMFLOAT3 mPlayerPos;
	XMFLOAT3 mLastPlayerPos;

	GameTimer* mTimer;
};

#endif