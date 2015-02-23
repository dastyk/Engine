#include "SnowEffect.h"


SnowEffect::SnowEffect() : ParticleSystemClass()
{

}


SnowEffect::~SnowEffect()
{

}

bool SnowEffect::Init(ID3D11Device* pDevice, XMFLOAT3* c, XMFLOAT3* e)
{


	Center = (*c);
	Extent = (*e);

	ParticleSystemClass::Init(pDevice, L"BTH_ny.jpg");

	return true;
}

void SnowEffect::Update(float dt)
{
	ParticleSystemClass::Update(dt);
}

void SnowEffect::createFirstParticles()
{


	for (int i = 0; i < 1000; i++)
	{
		XMFLOAT3 temp;
		temp.x = Center.x + rand() % (int)(Extent.x * 2) - Extent.x;
		temp.y = Center.y + rand() % (int)(Extent.y * 2) - Extent.y;
		temp.z = Center.z + rand() % (int)(Extent.z * 2) - Extent.z;
		
		mMoving.push_back(new ParticleClass(temp, XMFLOAT3(0, 0, 0), XMFLOAT3(0, -1, 0), 1, 0));
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
	if (pos.y < Center.y - Extent.y)
	{
		pos.x = Center.x + rand() % (int)(Extent.x * 2) - Extent.x;
		pos.y = Center.y + rand() % (int)(Extent.y * 2) - Extent.y;
		pos.z = Center.z + rand() % (int)(Extent.z * 2) - Extent.z;
		pParticle->GetTranform()->SetPosition(pos);

	}
}
