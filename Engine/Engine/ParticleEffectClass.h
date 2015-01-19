#ifndef _PARTICLEEFFECTCLASS_H_
#define _PARTICLEEFFECTCLASS_H_

#pragma once

#include "ParticleClass.h"
#include <vector>


class ParticleEffectClass
{
public:
	ParticleEffectClass();
	virtual ~ParticleEffectClass();

	bool Init();

	void Update(float dt);
	virtual void createFirstParticles() = 0;
	virtual void createDuringRun() = 0;

protected:
	std::vector<ParticleClass*> mEmitters;
	std::vector<ParticleClass*> mConstant;
	std::vector<ParticleClass*> mMoving;

};

#endif