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
	~ParticleSystemClass();

	bool Init(ID3D11Device*);

	void Update(float dt);
	void render(ID3D11DeviceContext* pDeviceContext);
	int GetAliveParticles()const;
	TextureClass* GetTexture()const;

private:
	std::vector<ParticleClass*> mParticle;

	void LoadParticlesToBuffer(ID3D11DeviceContext* pDeviceContext);

	ID3D11Buffer* mVertexBuffer;
	int mVertexCount;
	TextureClass* mTexture;
};

#endif