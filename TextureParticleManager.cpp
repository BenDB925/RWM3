#include "TextureParticleManager.h"
#include "FramerateCounter.h"


TextureParticleManager::TextureParticleManager(ParticleSettings pSettings)
	:_particleList(std::vector<ParticleObj>()),
	_positionToParentTo(pSettings._positionToParentTo),
	_velocity(0,0),
	_particleBaseVel(pSettings._velocity),
	_particleVelVariation(pSettings._velVariation),
	_particleTimeToLive(pSettings._timeToLive),
	_particleTexture(pSettings._texture),
	_particleSize(4, 4),
	_accel(0.5f),
	_goal(600, 600),
	_emissionRate(pSettings._emissionRate),
	_timeSinceEmit(0)

{

}

TextureParticleManager::~TextureParticleManager()
{
}

void TextureParticleManager::update(float pDT)
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
	
	if(_timeSinceEmit > _emissionRate)
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

void TextureParticleManager::render(SDL_Renderer * pRenderer)
{
	for (int i = 0; i < _particleList.size(); i++)
	{
			_particleList[i].render(pRenderer);
	}
}

void TextureParticleManager::SpawnParticle(Vector2 pDir)
{
	float randX = (static_cast<float>((rand() % 1000) / 1000.0f) * _particleVelVariation.x) - _particleVelVariation.x / 2;
	float randY = (static_cast<float>((rand() % 1000) / 1000.0f) * _particleVelVariation.y) - _particleVelVariation.y / 2;

	float xVel = (pDir.x - randX) * 150;
	float yVel = (pDir.y - randY) * 150;

	Vector2 vel = Vector2(xVel, yVel);

	float timeToLive = (static_cast<float>((rand() % 1000) / 1000.0f) * (_particleTimeToLive.y - _particleTimeToLive.x)) + _particleTimeToLive.x;
	_particleList.push_back(ParticleObj(_position, _particleSize, vel, timeToLive, _particleTexture));
}
