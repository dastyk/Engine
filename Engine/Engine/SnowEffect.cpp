#include "SnowEffect.h"


SnowEffect::SnowEffect() : ParticleSystemClass()
{
	mTimer = 0;
}


SnowEffect::~SnowEffect()
{
	if (mTimer)
	{
		delete mTimer;
		mTimer = 0;
	}
}

bool SnowEffect::Init(ID3D11Device* pDevice)
{
	ParticleSystemClass::Init(pDevice, L"BTH_ny.jpg");

	mTimer = new GameTimer();

	mTimer->Reset();
	mTimer->Start();

	return true;
}

void SnowEffect::Update(float dt)
{
	ParticleSystemClass::Update(dt);

	mTimer->Tick();
}

void SnowEffect::createFirstParticles()
{
	for (int i = 0; i < 1000; i++)
	{
		XMFLOAT3 temp;
		temp.x =(rand() % 256);
		temp.y =(rand() % 300);
		temp.z = (rand() % 256);
		
		mMoving.push_back(new ParticleClass(temp, XMFLOAT3(1, 1, 1), XMFLOAT3(0, -1, 0), 1, 0));
	}
}
void SnowEffect::CreateConstantInUpdate(ParticleClass* pParticle)
{

	
}

void SnowEffect::CreateEmitterInUpdate(ParticleClass* pParticle)
{


}

void SnowEffect::CreateMovingInUpdate(ParticleClass* pParticle)
{
	XMFLOAT3 pos = pParticle->GetTranform()->GetPosition();
	if (pos.y < 0)
	{

		pos.x = (rand() % 256);
		pos.y = (rand() % 300);
		pos.z = (rand() % 256);
		pParticle->GetTranform()->SetPosition(pos);

	}
}

void SnowEffect::SetPlayerPos(XMFLOAT3& position)
{
	mLastPlayerPos = mPlayerPos;
	mPlayerPos = position;
}