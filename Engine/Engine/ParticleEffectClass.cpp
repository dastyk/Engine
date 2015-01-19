#include "ParticleEffectClass.h"


ParticleEffectClass::ParticleEffectClass()
{

}

bool ParticleEffectClass::Init()
{
	createFirstParticles();

	return true;
}

void ParticleEffectClass::Update(float dt)
{


	createDuringRun();
}


ParticleEffectClass::~ParticleEffectClass()
{
}
