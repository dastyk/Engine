#include "SnowEffect.h"


SnowEffect::SnowEffect() : ParticleSystemClass()
{
}


SnowEffect::~SnowEffect()
{
}

void SnowEffect::createFirstParticles()
{
	for (int i = 0; i < 10000; i++)
	{
		XMFLOAT3 temp;
		temp.x = rand() % 128 * 2;
		temp.y = rand() % 300 + 80;
		temp.z = rand() % 128 * 2;
		mConstant.push_back(new ParticleClass(temp, XMFLOAT3(1, 0, 0), XMFLOAT3(0, -1, 0), 1, 0));
	}
}
void SnowEffect::CreateConstantInUpdate(ParticleClass* pParticle)
{
	XMFLOAT3 pos = pParticle->GetTranform()->GetPosition();
	if ( pos.y < 60)
	{
		pos.y = rand() % 300 + 80;
		pParticle->GetTranform()->SetPosition(pos);
	}
}

void SnowEffect::CreateEmitterInUpdate(ParticleClass* pParticle)
{


}

void SnowEffect::CreateMovingInUpdate(ParticleClass* pParticle)
{

}