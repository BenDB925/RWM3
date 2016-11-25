#pragma once
#include <SDL.h>
#include "Timer.h"
#include <sstream>

class FramerateCounter
{
public:
	FramerateCounter();
	~FramerateCounter();

	void update(SDL_Renderer * pRenderer);

	Timer _fpsTimer;

	static float _DT;

private:

	SDL_Color _textColor = { 0, 0, 0, 255 };
	int _countedFrames;
	float _timeInCurrSec;

	//In memory text stream
	std::string _timeText;

};