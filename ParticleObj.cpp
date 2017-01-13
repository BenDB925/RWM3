#include "ParticleObj.h"
#include "FramerateCounter.h"
#include "Shape.h"


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

ParticleObj::ParticleObj(Vector2 pPos, Vector2 pSize, Vector2 pVel, float pTimeToLive, Shape* pShape)
	:
	_position(pPos),
	_rect(SDL_Rect()),
	_timeAlive(0),
	_shape(pShape),
	_texture(nullptr),
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
	_timeAlive += FramerateCounter::_DT;
	_position.x += _velocity.x * FramerateCounter::_DT;
	_position.y += _velocity.y * FramerateCounter::_DT;

	if(_shape != nullptr)
	{
		_shape->Update(_velocity * FramerateCounter::_DT);
	}
}

bool ParticleObj::readyToRespawn() const
{
	return (_timeAlive > _timeToLive);
}

void ParticleObj::render(SDL_Renderer * pRenderer)
{
	if(_texture != nullptr)
	{
		_rect.x = _position.x;
		_rect.y = _position.y;
		SDL_SetTextureAlphaMod(_texture, 255 - ((_timeAlive / _timeToLive) * 255));

		SDL_RenderCopy(pRenderer, _texture, NULL, &_rect);
	}
	else
	{
		_shape->Draw();
	}
}
