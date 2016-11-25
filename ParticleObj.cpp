#include "ParticleObj.h"
#include "FramerateCounter.h"


ParticleObj::ParticleObj(Vector2 pPos, Vector2 pSize, Vector2 pVel, float pTimeToLive, SDL_Texture * pTexture)
	:
	_position(pPos), 
	_rect(SDL_Rect()), 
	_timeAlive(0),
	_texture(pTexture),
	_velocity(pVel),
	_timeToLive(pTimeToLive)
{
	_rect.x = pPos.x;
	_rect.y = pPos.y;
	_rect.w = pSize.x;
	_rect.h = pSize.y;
}


ParticleObj::~ParticleObj()
{
}

void ParticleObj::update()
{
	_velocity.x *= 1.00002f;

	_timeAlive += FramerateCounter::_DT;
	_position.x += _velocity.x * FramerateCounter::_DT;
	_position.y += _velocity.y * FramerateCounter::_DT;
}

bool ParticleObj::readyToRespawn() const
{
	return (_timeAlive > _timeToLive);
}

void ParticleObj::render(SDL_Renderer * pRenderer)
{
	_rect.x = _position.x;
	_rect.y = _position.y;
	SDL_RenderCopy(pRenderer, _texture, NULL, &_rect);
}
