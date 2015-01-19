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

private:
	void createFirstParticles();

	void CreateConstantInUpdate(ParticleClass*);
	void CreateEmitterInUpdate(ParticleClass*);
	void CreateMovingInUpdate(ParticleClass*);
};

#endif