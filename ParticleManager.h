#pragma once
#include <vector>
#include "ParticleObj.h"
#include <SDL_image.h>

class ParticleManager
{
public:
	ParticleManager(){}
	ParticleManager(Vector2 pPos, Vector2 pVel, Vector2 pVelVariation, int pParticleCount, Vector2 pTimeToLive, SDL_Texture * pTexture);
	~ParticleManager();

	void update();
	void followMouse(int pMouseX, int pMouseY);
	void render(SDL_Renderer * pRenderer);

private:

	void SpawnParticle();

	std::vector<ParticleObj> _particleList;
	Vector2 _position;
	Vector2 _velocity;
	Vector2 _particleBaseVel;
	Vector2 _particleVelVariation;
	Vector2 _particleSize;
	SDL_Texture * _particleTexture;
	Vector2 _particleTimeToLive;
	int _particleCount;
};

