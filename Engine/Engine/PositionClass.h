#ifndef _POSIITONCLASS_H_
#define _POSIITONCLASS_H_

#pragma once
#define SAMPLES_POSITION 5

#include <DirectXMath.h>

using namespace DirectX;

class PositionClass
{
public:
	PositionClass();
	PositionClass(const PositionClass* other);
	PositionClass(XMFLOAT3& Position, XMFLOAT3& Rotation);
	PositionClass(XMFLOAT3& Position, XMFLOAT3& Rotation, float moveSpeed);
	virtual ~PositionClass();

public:
	void SetPosition(XMFLOAT3& pos);
	void SetPosition(XMVECTOR& pos);
	virtual void SetRotation(XMFLOAT3& rot);
	
	XMFLOAT3 GetPosition()const;
	XMFLOAT3 GetRotation()const;

	float GetAvgPosY()const;

	void SetUpdateTime(float dt);
	void SetMoveSpeed(float speed);

	void MoveForward(bool);
	void MoveBackward(bool);
	void MoveUpward(bool);
	void MoveDownward(bool);
	void MoveRight(bool);
	void MoveLeft(bool);
	void MoveAlongVector(XMVECTOR* dir);


	void TurnLeft(bool);
	void TurnRight(bool);
	void LookUpward(bool);
	void LookDownward(bool);

	void CalcForwardUpRightVector();
protected:
	XMFLOAT3 mPosition;
	XMFLOAT3 mRotation;
	XMVECTOR mForward;
	XMVECTOR mUpVector;
	XMVECTOR mRightVector;

	XMFLOAT3 mLastPos[SAMPLES_POSITION];
	int mLastPosIndex;


	float mUpdateTime;

	float mMoveSpeed, mLookSpeed;
};

#endif