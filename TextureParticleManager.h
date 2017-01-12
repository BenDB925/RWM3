#pragma once
#include <vector>
#include "ParticleObj.h"
#include <SDL_image.h>

class TextureParticleManager
{
public:

	struct ParticleSettings
	{
		Vector2 * _positionToParentTo;
		Vector2 _offsetFromParent;
		Vector2 _velocity;
		Vector2 _velVariation;
		float _emissionRate;
		Vector2 _timeToLive;
		SDL_Texture * _texture;
	};

	static ParticleSettings _ROCKET_THRUSTER_PRESET;
	static ParticleSettings _FOOTSTEPS_PRESET;
	

	TextureParticleManager(){}
	TextureParticleManager(ParticleSettings pSettings);
	~TextureParticleManager();

	void update(float pDT);
	void followMouse(int pMouseX, int pMouseY);
	void render(SDL_Renderer * pRenderer);
private:

	void SpawnParticle(Vector2 pDir);


	std::vector<ParticleObj> _particleList;
	std::vector<Vector2> _velocityList;
	Vector2 * _positionToParentTo;
	Vector2 _position;
	Vector2 _offset;
	Vector2 _velocity;
	float _accel;
	Vector2 _goal;
	Vector2 _particleBaseVel;
	Vector2 _particleVelVariation;
	Vector2 _particleSize;
	SDL_Texture * _particleTexture;
	Vector2 _particleTimeToLive;
	float _emissionRate;
	float _timeSinceEmit;
};

