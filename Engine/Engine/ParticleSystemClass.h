#ifndef _PARTICLESYSTEMCLASS_H_
#define _PARTICLESYSTEMCLASS_H_

#pragma once
#include "ParticleClass.h"
#include <vector>
#include <d3d11.h>
#include "TextureClass.h"

struct ParticleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Color;
};

class ParticleSystemClass
{
public:
	ParticleSystemClass();
	virtual ~ParticleSystemClass();

	virtual bool Init(ID3D11Device*);

	virtual void Update(float dt);
	

	void render(ID3D11DeviceContext* pDeviceContext);
	int GetAliveParticles()const;
	TextureClass* GetTexture()const;

protected:
	std::vector<ParticleClass*> mConstant;
	std::vector<ParticleClass*> mMoving;
	std::vector<ParticleClass*> mEmitters;

	void LoadParticlesToBuffer(ID3D11DeviceContext* pDeviceContext);

	ID3D11Buffer* mVertexBuffer;
	int mVertexCount;
	TextureClass* mTexture;

	virtual void createFirstParticles() = 0;

	virtual void CreateConstantInUpdate(ParticleClass*) = 0;
	virtual void CreateEmitterInUpdate(ParticleClass*) = 0;
	virtual void CreateMovingInUpdate(ParticleClass*) = 0;

};

#endif