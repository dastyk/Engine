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

	bool Init(ID3D11Device*, XMFLOAT3* c, XMFLOAT3* e);
	void Update(float dt);

private:
	void createFirstParticles();

	void CreateConstantInUpdate(ParticleClass*);
	void CreateEmitterInUpdate(ParticleClass*);
	void CreateMovingInUpdate(ParticleClass*);


	XMFLOAT3 Center;
	XMFLOAT3 Extent;
};

#endif