#include "ParticleClass.h"

ParticleClass::ParticleClass()
{
	mTransform = new TransformationClass(XMFLOAT3(0,0,0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0));
	mColor = XMFLOAT3(0,0,0);
	mTravelDirection = XMFLOAT3(0, 0, 0);
	mMaxLifeTime = 0;
	mLifeTime = new GameTimer();
	mLifeTime->Reset();
	mTransform->SetMoveSpeed(0);
}

ParticleClass::ParticleClass(XMFLOAT3& Position, XMFLOAT3& Color, XMFLOAT3& travelDir, float moveSpeed, int maxLifeTime)
{
	mTransform = new TransformationClass(Position, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0));
	mColor = Color;
	mTravelDirection = travelDir;
	mMaxLifeTime = maxLifeTime;
	mLifeTime = new GameTimer();
	mLifeTime->Reset();
	mLifeTime->Start();
	mTransform->SetMoveSpeed(moveSpeed);
}

ParticleClass::ParticleClass(const ParticleClass& other)
{
	mTransform = new TransformationClass(other.mTransform);
	mColor = other.mColor;
	mTravelDirection = other.mTravelDirection;
	mMaxLifeTime = 40;
	mLifeTime = new GameTimer();
	mLifeTime->Reset();
	mLifeTime->Start();
	mTransform->SetMoveSpeed(mMaxLifeTime/10.0f);
}

ParticleClass::ParticleClass(const ParticleClass* other)
{
	mTransform = new TransformationClass(other->mTransform);
	mColor = other->mColor;
	mTravelDirection = other->mTravelDirection;
	mMaxLifeTime = 40;
	mLifeTime = new GameTimer();
	mLifeTime->Reset();
	mLifeTime->Start();
	mTransform->SetMoveSpeed(10 / (float)mMaxLifeTime);
}

ParticleClass::ParticleClass(const ParticleClass* other, XMFLOAT3& travelDir,float moveSpeed, int lifeTime)
{
	mTransform = new TransformationClass(other->mTransform);
	mColor = other->mColor;
	mTravelDirection = travelDir;
	mMaxLifeTime = lifeTime;
	mLifeTime = new GameTimer();
	mLifeTime->Reset();
	mLifeTime->Start();
	mTransform->SetMoveSpeed(moveSpeed);
}

ParticleClass::~ParticleClass()
{
	if (mTransform)
	{
		delete mTransform;
		mTransform = 0;
	}
	if (mLifeTime)
	{
		delete mLifeTime;
		mLifeTime = 0;
	}
}

void ParticleClass::Update(float dt)
{
	mLifeTime->Tick();
	if (mTravelDirection.x || mTravelDirection.y || mTravelDirection.z)
	{
		mTransform->SetUpdateTime(dt);
		mTransform->MoveAlongVector(mTravelDirection);
	}
	if (mMaxLifeTime)
	{
		float c = dt / mMaxLifeTime;
		mColor.x -= c;
		mColor.y += c;
		mColor.z += c*3;
	}
}

XMFLOAT3 ParticleClass::GetColor()const
{
	return mColor;
}

TransformationClass* ParticleClass::GetTranform()const
{
	return mTransform;
}

bool ParticleClass::isAlive()const
{
	if (mMaxLifeTime == 0)
		return true;


	if (mLifeTime->TotalTime() > 0)
	{
		int test = (int)(mLifeTime->TotalTime());
		int rnd = rand() % (int)(mLifeTime->TotalTime() + 1) + 1;
		if (rnd >= (mMaxLifeTime / 10.0f) * 3)
		{
			return false;
		}
	}
	return true;
}
