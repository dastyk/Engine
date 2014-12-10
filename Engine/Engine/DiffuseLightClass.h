#ifndef _DIFFUSELIGHTCLASS_H_
#define _DIFFUSELIGHTCLASS_H_


#pragma once
#include "LightClass.h"



class DiffuseLightClass :
	public LightClass
{
public:
	DiffuseLightClass();
	DiffuseLightClass(XMFLOAT3& dir, XMFLOAT3& color, XMFLOAT3& pos);
	~DiffuseLightClass();
};




#endif