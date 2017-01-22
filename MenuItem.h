#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include "ParticleManager.h"
class Vector2;

class MenuItem
{
public:
	MenuItem(Vector2 pPos, std::string pVarMess, SDL_Renderer * pRenderer, std::string(*pFunc)(ParticleManager*, bool), ParticleManager * pParticleManager);
	~MenuItem();

	void Draw();
	void ChangeText(bool pIsPositive);

	SDL_Rect _varRect;

	std::string(*_func)(ParticleManager*, bool);

private:
	TTF_Font* _font;
	SDL_Surface * _varSurface;
	SDL_Texture * _varTexture;
	std::string _varMess;
	SDL_Renderer * _renderer;
	ParticleManager * _particleManager;
};

