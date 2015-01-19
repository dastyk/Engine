#ifndef _FIREWORKEFFECT_H_
#define _FIREWORKEFFECT_H_

#pragma once
#include "ParticleSystemClass.h"

class FireworkEffect :
	public ParticleSystemClass
{
public:
	FireworkEffect();
	~FireworkEffect();

	bool Init(ID3D11Device*);
	void Update(float dt);

private:
	void createFirstParticles();

	void CreateConstantInUpdate(ParticleClass*);
	void CreateEmitterInUpdate(ParticleClass*);
	void CreateMovingInUpdate(ParticleClass*);

	GameTimer* mTimer;
};

#endif