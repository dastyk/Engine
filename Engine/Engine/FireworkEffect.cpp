#include "FireworkEffect.h"


FireworkEffect::FireworkEffect() : ParticleSystemClass()
{
	mTimer = 0;
}


FireworkEffect::~FireworkEffect()
{
	if (mTimer)
	{
		delete mTimer;
		mTimer = 0;
	}
}

bool FireworkEffect::Init(ID3D11Device* pDevice)
{
	ParticleSystemClass::Init(pDevice);

	mTimer = new GameTimer();
	
	mTimer->Reset();
	mTimer->Start();

	return true;
}

void FireworkEffect::Update(float dt)
{
	ParticleSystemClass::Update(dt);

	mTimer->Tick();
}

void FireworkEffect::createFirstParticles()
{
	mConstant.push_back(new ParticleClass(XMFLOAT3(128, 80, 128), XMFLOAT3(1, 0, 0), XMFLOAT3(0, 1, 0), 10, 0));
}
void FireworkEffect::CreateConstantInUpdate(ParticleClass* pParticle)
{
	if (rand() % 100 < 5)
	{
		XMFLOAT3 temp = pParticle->GetTranform()->GetPosition();
		temp.x += rand() % 40 - 20;
		temp.y += rand() % 20 - 10;
		temp.z += rand() % 40 - 20;
		mEmitters.push_back(new ParticleClass(temp, pParticle->GetColor(), XMFLOAT3(0, 0, 0), 0, 1));
	}
}

void FireworkEffect::CreateEmitterInUpdate(ParticleClass* pParticle)
{

	for (int i = 0; i < 20; i++)
	{
		XMVECTOR dir = XMVectorSet(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 50, 0);
		dir = XMVector3Normalize(dir);
		XMFLOAT3 fDir;
		XMStoreFloat3(&fDir, dir);
		mMoving.push_back(new ParticleClass(pParticle, fDir, 1, 10));
	}

}

void FireworkEffect::CreateMovingInUpdate(ParticleClass* pParticle)
{
	
}