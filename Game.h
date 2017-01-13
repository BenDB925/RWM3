#ifndef GAME_H
#define GAME_H
#include <SDL.h>
#include "FramerateCounter.h"
#include "ParticleMan.h"
#include "Vector2.h"

class Game
{
public:
	Game();
	~Game();
	bool Initialize(const char*, int, int, int, int, int);
	void LoadContent();
	void UnloadContent();
	void Render();
	void Update();
	void HandleEvents();
	bool IsRunning();
	void CleanUp();
private:
	Vector2 * _mousePos;
	bool m_running;
	SDL_Window* _window;
	SDL_Renderer* _renderer;
	SDL_Surface * _surface;

	FramerateCounter _frameCounter;
	ParticleMan _particleSys;
};
#endif