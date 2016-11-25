#pragma once
#include <SDL.h>
#include "Vector2.h"

class ParticleObj
{
public:
	ParticleObj(Vector2 pPos, Vector2 pSize, Vector2 pVel, float pTimeToLive, SDL_Texture * pTexture);
	~ParticleObj();
	void update();
	bool readyToRespawn() const;
	void render(SDL_Renderer * pRenderer);

private:
	Vector2 _position;
	SDL_Rect _rect;
	Vector2 _velocity;
	
	float _timeToLive;
	float _timeAlive;
	float _alphaDecayRate;
	SDL_Texture * _texture;
};

