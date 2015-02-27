#include "GPUTimerClass.h"


GPUTimerClass::GPUTimerClass()
{
	mStart = 0;
	mStop = 0;
	mDisjoint = 0;
}


GPUTimerClass::~GPUTimerClass()
{
	if (mStart)
	{
		mStart->Release();
		mStart = 0;
	}
	if (mStop)
	{
		mStop->Release();
		mStop = 0;
	}
	if (mDisjoint)
	{
		mDisjoint->Release();
		mDisjoint = 0;
	}
}

bool GPUTimerClass::Init(ID3D11Device* pDevice)
{
	HRESULT hr;
	D3D11_QUERY_DESC desc;
	desc.Query = D3D11_QUERY_TIMESTAMP;
	desc.MiscFlags = 0;
	hr = pDevice->CreateQuery(&desc, &mStart);
	if (FAILED(hr))
	{
		MessageBox(0, L"Could not create D3D11_QUERY_TIMESTAMP.", 0, 0);
		return false;
	}

	hr = pDevice->CreateQuery(&desc, &mStop);
	if (FAILED(hr))
	{
		MessageBox(0, L"Could not create D3D11_QUERY_TIMESTAMP.", 0, 0);
		return false;
	}

	desc.Query = D3D11_QUERY_TIMESTAMP_DISJOINT;
	hr = pDevice->CreateQuery(&desc, &mDisjoint);
	if (FAILED(hr))
	{
		MessageBox(0, L"Could not create D3D11_QUERY_TIMESTAMP_DISJOINT.", 0, 0);
		return false;
	}
	return true;
}

void GPUTimerClass::TimeStart(ID3D11DeviceContext* pDeviceContext)
{
	pDeviceContext->Begin(mDisjoint);
	pDeviceContext->End(mStart);
}

void GPUTimerClass::TimeEnd(ID3D11DeviceContext* pDeviceContext)
{
	pDeviceContext->End(mStop);
	pDeviceContext->End(mDisjoint);
}

double GPUTimerClass::GetTime(ID3D11DeviceContext* pDeviceContext)
{
	while (pDeviceContext->GetData(mDisjoint, NULL, 0, 0) == S_FALSE);	D3D11_QUERY_DATA_TIMESTAMP_DISJOINT tsDisjoint;	pDeviceContext->GetData(mDisjoint, &tsDisjoint, sizeof(tsDisjoint), 0);	if (!(tsDisjoint.Disjoint))	{		UINT64 StartTime, StopTime;		pDeviceContext->GetData(mStart, &StartTime, sizeof(UINT64), 0);		pDeviceContext->GetData(mStop, &StopTime, sizeof(UINT64), 0);		return (float(StopTime - StartTime) / float(tsDisjoint.Frequency))*1000.0f;	}
	return -1;
}
