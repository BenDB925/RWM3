#ifndef GAME_H
#define GAME_H
#include <SDL.h>
#include "FramerateCounter.h"
#include "ParticleManager.h"
#include "Vector2.h"
#include "MenuItem.h"

class Game
{
public:
	Game();
	~Game();
	bool Initialize(const char*, int, int, int, int, int);
	void LoadContent();
	void UnloadContent();
	void Render();
	void FindParticleSystemShape();
	void Update();
	void HandleEvents();
	bool IsRunning();
	void CleanUp();
	static Vector2 * _mousePos;
	static Vector2 _tronPos;
private:
	bool m_running;
	SDL_Window* _window;
	SDL_Renderer* _renderer;
	SDL_Surface * _surface;

	SDL_Texture * _menuTex;
	FramerateCounter _frameCounter;
	ParticleManager _particleSys;

	enum TronDirection
	{
		Up,
		Down,
		Left,
		Right
	};

	TronDirection _direction;
	float _timeSinceDirChange;
	float _TIME_BEFORE_DIR_CHANGE = 0.15f;
	const float _TRON_VEL = 2.5f;

	std::vector<MenuItem> _emitorShapes;
	int _emitorShapeIndex = 0;
	const int _EMITORSHAPECOUNT = 5;
};
#endif