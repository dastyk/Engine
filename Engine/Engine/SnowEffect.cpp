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
		temp.x = mPlayerPos.x + (rand() % 200 - 100) / 10.0f;
		temp.y = mPlayerPos.y + (rand() % 500 - 250) / 10.0f;
		temp.z = mPlayerPos.z + (rand() % 200 - 100) / 10.0f;
		
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
	if (pos.y < 70)
	{

		pos.y = mPlayerPos.y + (rand() % 150 + 70) / 10.0f;
		pos.x = mPlayerPos.x + (rand() % 200 - 100) / 10.0f;
		pos.z = mPlayerPos.z + (rand() % 200 - 100) / 10.0f;
		pParticle->GetTranform()->SetPosition(pos);

	}
	if (mLastPlayerPos.x != mPlayerPos.x || mLastPlayerPos.y != mPlayerPos.y || mLastPlayerPos.z != mPlayerPos.z)
	{

		XMVECTOR player = XMLoadFloat3(&mPlayerPos);
		XMVECTOR vPos = XMLoadFloat3(&pos);
		XMVectorSetY(player, 0);
		XMVectorSetY(vPos, 0);

		float distance = XMVectorGetX(XMVectorAbs(player - vPos));
		if (distance > 10)
		{
			pos.x = mPlayerPos.x + (rand() % 200 - 100) / 10.0f;
			pos.z = mPlayerPos.z + (rand() % 200 - 100) / 10.0f;
			pParticle->GetTranform()->SetPosition(pos);
		}
	}
}

void SnowEffect::SetPlayerPos(XMFLOAT3& position)
{
	mLastPlayerPos = mPlayerPos;
	mPlayerPos = position;
}