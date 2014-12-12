#ifndef _POSIITONCLASS_H_
#define _POSIITONCLASS_H_

#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class PositionClass
{
public:
	PositionClass();
	PositionClass(const PositionClass* other);
	PositionClass(XMFLOAT3& Position, XMFLOAT3& Rotation);
	virtual ~PositionClass();

public:
	void SetPosition(XMFLOAT3& pos);
	virtual void SetRotation(XMFLOAT3& rot);
	
	XMFLOAT3 GetPosition()const;
	XMFLOAT3 GetRotation()const;

	void SetUpdateTime(float dt);
	void SetMoveSpeed(float speed);

	void MoveForward(bool);
	void MoveBackward(bool);
	void MoveUpward(bool);
	void MoveDownward(bool);
	void MoveRight(bool);
	void MoveLeft(bool);

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


	float mUpdateTime;

	float mMoveSpeed, mLookSpeed;
};

#endif