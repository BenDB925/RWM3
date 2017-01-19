#include "ParticleManager.h"
#include "FramerateCounter.h"


ParticleManager::ParticleManager(ParticleSettings pSettings, SDL_Renderer * pRenderer)
	:_particleList(std::vector<ParticleObj *>()),
	_positionToParentTo(pSettings._positionToParentTo),
	_velocity(0, 0),
	_particleBaseVel(pSettings._velocity),
	_particleVelVariation(pSettings._velVariation),
	_particleTimeToLive(pSettings._timeToLive),
	_particleTexture(pSettings._texture),
	_shapeType(pSettings._shapeType),
	_particleSize(10),
	_accel(0.5f),
	_emissionRate(pSettings._emissionRate),
	_timeSinceEmit(80),
	_renderer(pRenderer),
	_rotationMaxSpeed(pSettings._rotationMaxSpeed)

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
		_particleList[i]->update();

		if (_particleList[i]->readyToRespawn())
		{
			delete _particleList.at(i);
			_particleList.erase(_particleList.begin() + i);
		}
	}
}

void ParticleManager::render(SDL_Renderer * pRenderer)
{
	for (int i = 0; i < _particleList.size(); i++)
	{
		_particleList[i]->render(pRenderer);
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
	ParticleObj::ParticleObjSettings settings;

	settings._position = _position;
	settings._size = _particleSize;
	settings._velocity = vel;
	settings._timeToLive = timeToLive;
	settings._texture = _particleTexture;


	if (_shapeType != Shape::NULL_SHAPE)
	{
		std::vector<Vector2> vertPositions;
		Shape * shape = nullptr;
		Uint8 randR = rand() % 255;
		Uint8 randG = rand() % 255;
		Uint8 randB = rand() % 255;


		float rotSpeed = ((rand() % 1000 / 1000.0f) * _rotationMaxSpeed * 2) - _rotationMaxSpeed;

		switch (_shapeType)
		{
		case Shape::Triangle:
			vertPositions.push_back(Vector2(0, 0));
			vertPositions.push_back(Vector2(5, 0));
			vertPositions.push_back(Vector2(0, 5));

			shape = new Shape(_position, vertPositions, Shape::ShapeType::Triangle, _renderer, SDL_Colour{ randR, randG, randB, 255 }, rotSpeed);

			break;

		case Shape::Square:
			vertPositions.push_back(Vector2(-5,	-5));
			vertPositions.push_back(Vector2( 5,	-5));
			vertPositions.push_back(Vector2( 5,	 5));
			vertPositions.push_back(Vector2(-5,	 5));

			shape = new Shape(_position, vertPositions, Shape::ShapeType::Square, _renderer, SDL_Colour{ randR, randG, randB, 255 }, rotSpeed);
			break;

		case Shape::Pentagon:
			vertPositions.push_back(Vector2( 0	* _particleSize,  0 * -_particleSize));
			vertPositions.push_back(Vector2( 2	* _particleSize,  0 * -_particleSize));
			vertPositions.push_back(Vector2(2.6	* _particleSize, 2 * -_particleSize));
			vertPositions.push_back(Vector2(1	* _particleSize,  3.1 * -_particleSize));
			vertPositions.push_back(Vector2( -0.64	* _particleSize,  2 * -_particleSize));

			shape = new Shape(_position, vertPositions, Shape::ShapeType::Pentagon, _renderer, SDL_Colour{ randR, randG, randB, 255 }, rotSpeed);
			break;

		case Shape::Star:
						
			vertPositions.push_back(Vector2(6.5		*		_particleSize * 0.2f, 0		*		-_particleSize * 0.2f));
			vertPositions.push_back(Vector2(5		*		_particleSize * 0.2f, 5		*		-_particleSize * 0.2f));
			vertPositions.push_back(Vector2(0		*		_particleSize * 0.2f, 5.5f	*		-_particleSize * 0.2f));
			vertPositions.push_back(Vector2(3.5f	*		_particleSize * 0.2f, 9		*		-_particleSize * 0.2f));
			vertPositions.push_back(Vector2(2		*		_particleSize * 0.2f, 14	*		-_particleSize * 0.2f));
			vertPositions.push_back(Vector2(6.5f	*		_particleSize * 0.2f, 11.5f *		-_particleSize * 0.2f));
			vertPositions.push_back(Vector2(12		*		_particleSize * 0.2f, 14	*		-_particleSize * 0.2f));
			vertPositions.push_back(Vector2(10.5f	*		_particleSize * 0.2f, 9		*		-_particleSize * 0.2f));
			vertPositions.push_back(Vector2(14		*		_particleSize * 0.2f, 5.5f	*		-_particleSize * 0.2f));
			vertPositions.push_back(Vector2(9		*		_particleSize * 0.2f, 5		*		-_particleSize * 0.2f));


			shape = new Shape(_position, vertPositions, Shape::ShapeType::Star, _renderer, SDL_Colour{ randR, randG, randB, 255 }, rotSpeed);
			break;

		case Shape::NULL_SHAPE: 
			delete shape;
			break;
		default:

			break;
		}


		settings._shape = shape;
	}


	_particleList.push_back(new ParticleObj(settings));
}
