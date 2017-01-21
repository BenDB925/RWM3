#include "ParticleObj.h"
#include "FramerateCounter.h"
#include "Shape.h"
#include "ParticleManager.h"


ParticleObj::ParticleObj(ParticleObjSettings pSettings, ParticleManager & pParticleSys)
	:
	_parentSys(pParticleSys),
	_position(pSettings._position),
	_rect(SDL_Rect()),
	_velocity(pSettings._velocity),
	_timeToLive(pSettings._timeToLive),
	_timeAlive(0),
	_texture(pSettings._texture),
	_shape(pSettings._shape)
{
	_rect.x = _position.x;
	_rect.y = _position.y;
	_rect.w = pSettings._size.x;
	_rect.h = pSettings._size.y;
}

ParticleObj::~ParticleObj()
{
	delete _shape;
}

void ParticleObj::update()
{
	_timeAlive += FramerateCounter::_DT;

	if(_timeAlive > _timeToLive)
		return;

	_position.x += _velocity.x * FramerateCounter::_DT;
	_position.y += _velocity.y * FramerateCounter::_DT;

	if (_shape != nullptr)
	{
		SDL_Color col = _parentSys.GetColour(_timeAlive);

		if ((col.r == 0 && col.g == 0 && col.b == 0 && col.a == 0) == false)
			_shape->SetColour(col);

		_shape->Update(_velocity * FramerateCounter::_DT, FramerateCounter::_DT);
	}
}

bool ParticleObj::readyToRespawn() const
{
	return (_timeAlive > _timeToLive);
}

void ParticleObj::render(SDL_Renderer * pRenderer)
{
	if (_shape == nullptr)
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
