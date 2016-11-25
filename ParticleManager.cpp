#include "ParticleManager.h"



ParticleManager::ParticleManager(Vector2 pPos, Vector2 pVel, Vector2 pVelVariation, int pParticleCount, Vector2 pTimeToLive, SDL_Texture * pTexture)
	:_particleList(std::vector<ParticleObj>()),
	_position(pPos),
	_particleBaseVel(pVel),
	_particleVelVariation(pVelVariation),
	_particleCount(pParticleCount),
	_particleTimeToLive(pTimeToLive),
	_particleTexture(pTexture),
	_particleSize(4,4)

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
	Vector2 vecToMouse = Vector2(pMouseX - _position.x, pMouseY - _position.y);
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

	Vector2 vel = Vector2(
		(randX * _particleVelVariation.x) - (_particleVelVariation.x / 2) + _particleBaseVel.x,
		(randY * _particleVelVariation.y) - (_particleVelVariation.y / 2) + _particleBaseVel.y);

	float timeToLive = (static_cast<float>((rand() % 1000) / 1000.0f) * (_particleTimeToLive.y - _particleTimeToLive.x)) + _particleTimeToLive.x;
	_particleList.push_back(ParticleObj(_position, _particleSize, vel, timeToLive, _particleTexture));
}
