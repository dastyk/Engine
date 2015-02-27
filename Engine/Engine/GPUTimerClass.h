#ifndef _GPUTIMERCLASS_H_
#define _GPUTIMERCLASS_H_

#pragma once

#include <d3d11.h>
class GPUTimerClass
{
public:
	GPUTimerClass();
	~GPUTimerClass();

	bool Init(ID3D11Device* pDevice);

	void TimeStart(ID3D11DeviceContext* pDeviceContext);
	void TimeEnd(ID3D11DeviceContext* pDeviceContext);
	double GetTime(ID3D11DeviceContext* pDeviceContext);

private:

	ID3D11Query* mStart, *mStop, *mDisjoint;

};

#endif