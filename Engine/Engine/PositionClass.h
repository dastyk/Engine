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
	PositionClass(const XMFLOAT3& Position, const XMFLOAT3& Rotation);
	PositionClass(const XMFLOAT3& Position, const XMFLOAT3& Rotation, const float moveSpeed);
	virtual ~PositionClass();

public:
	void SetPosition(const XMFLOAT3& pos);
	void SetPosition(const XMVECTOR& pos);
	virtual void SetRotation(const XMFLOAT3& rot);


	XMFLOAT3 GetPosition()const;
	XMFLOAT3 GetRotation()const;
	XMFLOAT3 GetForward()const;

	float GetAvgPosY()const;

	void SetUpdateTime(const float dt);
	void SetMoveSpeed(const float speed);

	void MoveForward(bool);
	void MoveBackward(bool);
	void MoveUpward(bool);
	void MoveDownward(bool);
	void MoveRight(bool);
	void MoveLeft(bool);
	void MoveAlongVector(const XMFLOAT3& dir);


	void TurnLeft(bool);
	void TurnRight(bool);
	void LookUpward(bool);
	void LookDownward(bool);

	void CalcForwardUpRightVector();
protected:
	XMFLOAT3 mPosition;
	XMFLOAT3 mRotation;
	XMFLOAT3 mForward;
	XMFLOAT3 mUpVector;
	XMFLOAT3 mRightVector;

	XMFLOAT3 mLastPos[SAMPLES_POSITION];
	int mLastPosIndex;


	float mUpdateTime;

	float mMoveSpeed, mLookSpeed;
};

#endif