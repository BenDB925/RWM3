#include "ParticleManager.h"
#include "FramerateCounter.h"


ParticleManager::ParticleManager(ParticleSettings pSettings, SDL_Renderer * pRenderer)
	:_particleList(std::vector<ParticleObj>()),
	_positionToParentTo(pSettings._positionToParentTo),
	_velocity(0, 0),
	_particleBaseVel(pSettings._velocity),
	_particleVelVariation(pSettings._velVariation),
	_particleTimeToLive(pSettings._timeToLive),
	_particleTexture(pSettings._texture),
	_shapeType(pSettings._shapeType),
	_particleSize(4, 4),
	_accel(0.5f),
	_goal(600, 600),
	_emissionRate(pSettings._emissionRate),
	_timeSinceEmit(0),
	_renderer(pRenderer)

{

}

ParticleManager::~ParticleManager()
{
}

void ParticleManager::update(float pDT)
{
	Vector2 diff = _position - (*_positionToParentTo + _offset);

	if (diff.Length() > 2)
	{
		if (_velocityList.size() < 15)
			_velocityList.push_back(diff);
		else
		{
			_velocityList.insert(_velocityList.begin(), diff);
			_velocityList.pop_back();
		}
		Vector2 totalSpeed = Vector2(0, 0);
		for (int i = 0; i < _velocityList.size(); ++i)
		{
			totalSpeed = totalSpeed + _velocityList.at(i);
		}

		_velocity = totalSpeed / _velocityList.size();

		_position = *_positionToParentTo + _offset;
	}


	_timeSinceEmit += pDT;

	if (_timeSinceEmit > _emissionRate)
	{
		SpawnParticle(_velocity.Normalize());
		_timeSinceEmit = 0;
	}

	for (int i = 0; i < _particleList.size(); i++)
	{
		_particleList[i].update();

		if (_particleList[i].readyToRespawn())
		{
			_particleList.erase(_particleList.begin() + i);
		}
	}
}

void ParticleManager::render(SDL_Renderer * pRenderer)
{
	for (int i = 0; i < _particleList.size(); i++)
	{
		_particleList[i].render(pRenderer);
	}
}

void ParticleManager::SpawnParticle(Vector2 pDir)
{
	float randX = (static_cast<float>((rand() % 1000) / 1000.0f) * _particleVelVariation.x) - _particleVelVariation.x / 2;
	float randY = (static_cast<float>((rand() % 1000) / 1000.0f) * _particleVelVariation.y) - _particleVelVariation.y / 2;

	float xVel = (pDir.x - randX) * 150;
	float yVel = (pDir.y - randY) * 150;

	Vector2 vel = Vector2(xVel, yVel);

	float timeToLive = (static_cast<float>((rand() % 1000) / 1000.0f) * (_particleTimeToLive.y - _particleTimeToLive.x)) + _particleTimeToLive.x;

	if(_shapeType == Shape::NULL_SHAPE)
	{
		_particleList.push_back(ParticleObj(_position, _particleSize, vel, timeToLive, _particleTexture));
	}
	else
	{
		std::vector<Vector2> vertPositions;
		Shape * shape = nullptr;
		Uint8 randR = rand() % 255;
		Uint8 randG = rand() % 255;
		Uint8 randB = rand() % 255;

		switch(_shapeType)
		{
		case Shape::Triangle:
			vertPositions.push_back(Vector2(_position.x,		_position.y));
			vertPositions.push_back(Vector2(_position.x + 5,	_position.y));
			vertPositions.push_back(Vector2(_position.x,		_position.y + 5));



 			shape = new Shape(_position, vertPositions, Shape::ShapeType::Triangle, _renderer, SDL_Colour{ randR, randG, randB, 255 });

			break;
		default: 
			break;
		}

		if(shape != nullptr)
			_particleList.push_back(ParticleObj(_position, _particleSize, vel, timeToLive, shape));
	}
}
