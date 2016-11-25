#include "ParticleManager.h"
#include "FramerateCounter.h"


ParticleManager::ParticleManager(Vector2 pPos, Vector2 pVel, Vector2 pVelVariation, int pParticleCount, Vector2 pTimeToLive, SDL_Texture * pTexture)
	:_particleList(std::vector<ParticleObj>()),
	_position(pPos),
	_velocity(0,0),
	_particleBaseVel(pVel),
	_particleVelVariation(pVelVariation),
	_particleCount(pParticleCount),
	_particleTimeToLive(pTimeToLive),
	_particleTexture(pTexture),
	_particleSize(4,4),
	_accel(0.3),
	_goal(-1,-1)

{
	for (int i = 0; i < pParticleCount; i++)
	{
		SpawnParticle();
	}
}

ParticleManager::~ParticleManager()
{
}

void ParticleManager::update()
{
	Vector2 vecToGoal = Vector2(_goal.x - _position.x, _goal.y - _position.y);

	if(vecToGoal.Length() > 5 && (_goal.x != -1 && _goal.y != -1))
	{
		vecToGoal = vecToGoal.Normalize();

		_velocity.x += _accel * vecToGoal.x * FramerateCounter::_DT;
		_velocity.y += _accel * vecToGoal.y * FramerateCounter::_DT;

	}
	else
	{
		_velocity.x = 0;
		_velocity.y = 0;
	}

	_position.x += _velocity.x;
	_position.y += _velocity.y;

	for (int i = 0; i < _particleList.size(); i++)
	{
		_particleList[i].update();

		if (_particleList[i].readyToRespawn())
		{
			SpawnParticle();
			_particleList.erase(_particleList.begin() + i);
		}
	}
}

void ParticleManager::followMouse(int pMouseX, int pMouseY)
{
	_goal = Vector2(pMouseX, pMouseY);
}

void ParticleManager::render(SDL_Renderer * pRenderer)
{
	for (int i = 0; i < _particleCount; i++)
	{
		_particleList[i].render(pRenderer);
	}
}

void ParticleManager::SpawnParticle()
{

	float randX = static_cast<float>((rand() % 1000) / 1000.0f);
	float randY = static_cast<float>((rand() % 1000) / 1000.0f);

	//Vector2 vel = Vector2(
	//	(randX * _particleVelVariation.x) - (_particleVelVariation.x / 2) + _particleBaseVel.x,
	//	(randY * _particleVelVariation.y) - (_particleVelVariation.y / 2) + _particleBaseVel.y);

	Vector2 vel = Vector2(-_velocity.x * 100, -_velocity.y * 100);

	float timeToLive = (static_cast<float>((rand() % 1000) / 1000.0f) * (_particleTimeToLive.y - _particleTimeToLive.x)) + _particleTimeToLive.x;
	_particleList.push_back(ParticleObj(_position, _particleSize, vel, timeToLive, _particleTexture));
}
