#include "GameTimer.h"


GameTimer::GameTimer() : mSecondsPerCount(0.0), mDeltaTime(-1.0), mBaseTime(0), mPausedTime(0), mPrevTime(0), mCurrTime(0), mStopped(false)
{
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*) & countsPerSec);
	mSecondsPerCount = 1.0 / (double)countsPerSec;
}


GameTimer::~GameTimer()
{
}

void GameTimer::Tick()
{
	if (mStopped)
	{
		mDeltaTime = 0.0;
		return;
	}

	// Get time this frame
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	mCurrTime = currTime;

	// Time diff between this and previous frame.
	mDeltaTime = (mCurrTime - mPrevTime)*mSecondsPerCount;

	// Prepare for next frame
	mPrevTime = mCurrTime;

	if (mDeltaTime < 0.0)
		mDeltaTime = 0.0;

}

float GameTimer::DeltaTime() const
{
	return (float)mDeltaTime;
}

void GameTimer::Reset()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	mBaseTime = currTime;
	mPrevTime = currTime;
	mStopTime = 0;
	mStopped = false;
}

void GameTimer::Stop()
{
	if (!mStopped)
	{
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		mStopTime = currTime;
		mStopped = true;
	}
}

void GameTimer::Start()
{
	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	if (mStopped)
	{
		mPausedTime += startTime - mStopTime;

		mPrevTime = startTime;

		mStopTime = 0;
		mStopped = false;
	}
}

float GameTimer::TotalTime()const
{
	if (mStopped)
		return (float)(((mStopTime - mPausedTime) - mBaseTime)*mSecondsPerCount);
	else
		return (float)(((mCurrTime - mPausedTime) - mBaseTime)*mSecondsPerCount);
}