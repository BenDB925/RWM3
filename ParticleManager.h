#pragma once
#include <vector>
#include "ParticleObj.h"
#include "Shape.h"

class ParticleManager
{
public:

	struct ColourLerper
	{
		SDL_Color _colour;
		float _durationOfColour;
	};

	struct ParticleSettings
	{
		Vector2 * _positionToParentTo;
		Vector2 _offsetFromParent;
		Vector2 _velocity;
		Vector2 _velVariation;
		float _emissionRate;
		float _timeToLiveVariation;
		SDL_Texture * _texture;
		Shape::ShapeType _shapeType;
		std::vector<ColourLerper> _coloursToLerp;
		float _rotationMaxSpeed;
	};

	static ParticleSettings _ROCKET_THRUSTER_PRESET;
	static ParticleSettings _FOOTSTEPS_PRESET;

	SDL_Color GetColour(float pAliveTime);
	ParticleManager() {}
	ParticleManager(ParticleSettings pSettings, SDL_Renderer * pRenderer);
	~ParticleManager();

	void update(float pDT);
	void render(SDL_Renderer * pRenderer);
private:

	void SpawnParticle(Vector2 pDir);

	std::vector<ColourLerper> _colourLerpingList;
	std::vector<ParticleObj *> _particleList;
	std::vector<Vector2> _velocityList;
	Vector2 * _positionToParentTo;
	Vector2 _position;
	Vector2 _offset;
	Vector2 _velocity;
	float _accel;
	Vector2 _particleBaseVel;
	Vector2 _particleVelVariation;
	float _particleSize;
	SDL_Texture * _particleTexture;
	Vector2 _particleTimeToLive;
	float _emissionRate;
	float _timeSinceEmit;
	Shape::ShapeType _shapeType;
	SDL_Renderer * _renderer;
	float _rotationMaxSpeed;
};

