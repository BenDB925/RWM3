#include <SDL_hints.h>

class Timer
{
public:
	//Initializes variables
	Timer();
	~Timer();

	//The various clock actions
	void start();

	//Gets the timer's time
	Uint32 getTicks();

private:
	//The clock time when the timer started
	Uint32 mStartTicks;

	Uint32 mCurrTime;
	Uint32 mLastTicks;
};
