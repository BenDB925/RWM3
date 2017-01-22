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

	class ParticleSettings
	{
	public:
		ParticleSettings()
			:_positionToParentTo(nullptr),
			_offsetFromParent(Vector2(0, 0)),
			_startingVelocity(0),
			_endingVelocity(0),
			_velVariation(1),
			_emissionRate(0.02f),
			_timeToLiveVariation(0),
			_minTTL(2),
			_maxTTL(4),
			_texture(nullptr),
			_shapeType(Shape::ShapeType::Pentagon),
			_coloursToLerp(std::vector<ColourLerper>()),
			_rotationMaxSpeed(0)
		{

		}

		Vector2 * _positionToParentTo;
		Vector2 _offsetFromParent;
		float _startingVelocity;
		float _endingVelocity;
		float _velVariation;
		float _emissionRate;
		float _timeToLiveVariation;
		float _minTTL;
		float _maxTTL;
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
	std::string IncrementShapeType();
	std::string DecrementShapeType();


	//button functions
	float * GetEmissionRate();
	static std::string ChangeEmissionRate(ParticleManager * pManager, bool pIncrement);
	static std::string ChangeMinTimeToLive(ParticleManager * pManager, bool pIncrement);
	static std::string ChangeMaxTimeToLive(ParticleManager * pManager, bool pIncrement);
	static std::string ChangeStartingVelocity(ParticleManager * pManager, bool pIncrement);
	static std::string ChangeEndingVelocity(ParticleManager * pManager, bool pIncrement);
	static std::string ChangeConeAngle(ParticleManager * pManager, bool pIncrement);
	static std::string ChangeParticleType(ParticleManager * pManager, bool pIncrement);
	static std::string ChangeParticleSize(ParticleManager * pManager, bool pIncrement);
	static std::string ChangeStartingRColour(ParticleManager * pManager, bool pIncrement);
	static std::string ChangeStartingGColour(ParticleManager * pManager, bool pIncrement);
	static std::string ChangeStartingBColour(ParticleManager * pManager, bool pIncrement);
	static std::string ChangeStartingAColour(ParticleManager * pManager, bool pIncrement);
	static std::string ChangeEndingRColour(ParticleManager * pManager, bool pIncrement);
	static std::string ChangeEndingGColour(ParticleManager * pManager, bool pIncrement);
	static std::string ChangeEndingBColour(ParticleManager * pManager, bool pIncrement);
	static std::string ChangeEndingAColour(ParticleManager * pManager, bool pIncrement);

	float _minTTL;
	float _maxTTL;
	float _particleSize;
	std::vector<ColourLerper> _colourLerpingList;
private:

	void SpawnParticle(Vector2 pDir);

	std::vector<ParticleObj *> _particleList;
	std::vector<Vector2> _velocityList;
	Vector2 * _positionToParentTo;
	Vector2 _position;
	Vector2 _offset;
	Vector2 _currentVelocity;
	float _startingVelocity;
	float _endingVelocity;
	float _accel;
	float _particleVelVariation;
	SDL_Texture * _particleTexture;
	float _timeSinceEmit;
	Shape::ShapeType _shapeType;
	SDL_Renderer * _renderer;
	float _rotationMaxSpeed;
	float _emissionRate;
};

