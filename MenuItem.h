#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include "ParticleManager.h"
class Vector2;

class MenuItem
{
public:
	MenuItem(Vector2 pPos, std::string pMessage, float* pVariableToChange, float pChangingIncrement, SDL_Renderer* pRenderer, void (*pFunc)(ParticleManager*, bool), ParticleManager* pParticleManager);
	~MenuItem();

	void Draw();
	void ChangeText(bool pIsPositive);

	SDL_Rect _textRect;

	void(*_func)(ParticleManager*, bool);

private:
	float * _variableToChange;
	float _amountToIncrementVar;
	TTF_Font* _font;
	SDL_Surface * _textSurface;
	SDL_Texture * _textTexture;
	SDL_Rect _varRect;
	SDL_Surface * _varSurface;
	SDL_Texture * _varTexture;
	std::string _varMess;
	SDL_Renderer * _renderer;
	ParticleManager * _particleManager;
};

