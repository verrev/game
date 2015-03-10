#include "CTimer.h"
float CTimer::mSecondsPerCount = 0;
__int64 CTimer::mA = 0;
__int64 CTimer::mB = 0;
float CTimer::mDt = 0;
float CTimer::mElapsedTime = 0.0f;
float CTimer::mNumFrames = 0.0f;
float CTimer::mFPS = 0.0f;
float CTimer::mMsPerFrame = 0.0f;
bool CTimer::init()
{
	mDt = 1.0f;
	__int64 countsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	mSecondsPerCount = 1.0f / (float)countsPerSec;
	if (mSecondsPerCount)
		return 1;
	return 0;
}
void CTimer::begin()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&mA);
}
void CTimer::end(const bool &stats, const float &timeStep)
{
	QueryPerformanceCounter((LARGE_INTEGER*)&mB);
	mDt = (mB - mA)*mSecondsPerCount;
	if (stats){
		mNumFrames++;
		computeStats(timeStep);
	}
}
float CTimer::getDelta()
{
	return mDt;
}
FrameStats CTimer::getStats()
{
	FrameStats fs = { mFPS, mMsPerFrame };
	return fs;
}
void CTimer::computeStats(const float &timeStep)
{
	mElapsedTime += mDt;
	if (mElapsedTime >= timeStep){
		mFPS = mNumFrames;
		mNumFrames = 0.0f;
		mElapsedTime = 0.0f;
		mMsPerFrame = 1000.0f / mFPS;
	}
}