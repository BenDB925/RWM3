#include "Timer.h"
#include <SDL.h>


Timer::Timer()
{
	//Get the current clock time
	mStartTicks = 0;
	mLastTicks = mStartTicks;
	mCurrTime = mStartTicks;
}

Timer::~Timer()
{
}

void Timer::start()
{
	mStartTicks = 0;
}

Uint32 Timer::getTicks()
{
	//The actual timer time
	Uint32 timeSinceStart = 0;

	timeSinceStart = SDL_GetTicks() - mStartTicks;
	mLastTicks = mCurrTime;
	mCurrTime = timeSinceStart;
	Uint32 diff = mCurrTime - mLastTicks;

	return diff;
}