#pragma once
#include <SDL.h>
#include "Vector2.h"

class Shape;
class ParticleManager;

class ParticleObj
{
public:

struct ParticleObjSettings
{
	Vector2 _position;
	Vector2 _size;
	SDL_Rect _rect;
	Vector2 _velocity;
	float _timeToLive;
	SDL_Texture * _texture;
	Shape * _shape;
};


	ParticleObj(ParticleObjSettings pSettings, ParticleManager & pParticleSys);
	~ParticleObj();
	void update();
	bool readyToRespawn() const;
	void render(SDL_Renderer * pRenderer);

private:
	ParticleManager & _parentSys;
	Vector2 _position;
	SDL_Rect _rect;
	Vector2 _velocity;
	float _timeToLive;
	float _timeAlive;
	float _alphaDecayRate;
	SDL_Texture * _texture;
	Shape * _shape;
};

