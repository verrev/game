#ifndef _CTIMER_H_
#define _CTIMER_H_
#include <windows.h>
struct FrameStats
{
public:
	float mFPS;
	float mMsPerFrame;
};
class CTimer
{
public:
	static bool init();
	static void begin();
	static void end(const bool &stats = 1, const float &timeStep = 1.0f);
	static float getDelta();
	static FrameStats getStats();
private:
	static void computeStats(const float &timeStep);
private:
	static float mSecondsPerCount;
	static __int64 mA;
	static __int64 mB;
	static float mDt;

	static float mElapsedTime;
	static float mNumFrames;
	static float mFPS;
	static float mMsPerFrame;
};
#endif