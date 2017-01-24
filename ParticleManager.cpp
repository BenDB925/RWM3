#pragma once
#include "ParticleManager.h"
#include "FramerateCounter.h"
#include "TextureLoader.h"
#include "Game.h"

ParticleManager::ParticleSettings ParticleManager::_FOOTSTEPS_PRESET;
ParticleManager::ParticleSettings ParticleManager::_ROCKET_THRUSTER_PRESET;
ParticleManager::ParticleSettings ParticleManager::_TRON_PRESET;

ParticleManager::ParticleManager(ParticleSettings pSettings, SDL_Renderer * pRenderer)
	:_particleSize(pSettings._particleSize),
	_colourLerpingList(pSettings._coloursToLerp),
	_started(true),
	_particleList(std::vector<ParticleObj *>()),
	_positionToParentTo(pSettings._positionToParentTo),
	_currentVelocity(0, 0),
	_startingVelocity(pSettings._startingVelocity),
	_endingVelocity(pSettings._endingVelocity),
	_accel(0.5f),
	_particleVelVariation(pSettings._velVariation),
	_textureExampleHolder(pSettings._texture),
	_timeSinceEmit(0),
	_shapeType(pSettings._shapeType),
	_renderer(pRenderer),
	_rotationMaxSpeed(pSettings._rotationMaxSpeed),
	_emissionRate(pSettings._emissionRate)
{
	float timeToLiveTotal = 0;

	for (int i = 0; i < _colourLerpingList.size(); ++i)
	{
		timeToLiveTotal += _colourLerpingList.at(i)._durationOfColour;
	}

	_minTTL = timeToLiveTotal - pSettings._timeToLiveVariation;
	_maxTTL = timeToLiveTotal;


	_FOOTSTEPS_PRESET = ParticleSettings();
	_FOOTSTEPS_PRESET._particleSize = 25;
	_FOOTSTEPS_PRESET._emissionRate = 0.424f;
	_FOOTSTEPS_PRESET._startingVelocity = 0;
	_FOOTSTEPS_PRESET._endingVelocity = 0;

	_ROCKET_THRUSTER_PRESET = ParticleSettings();
	_ROCKET_THRUSTER_PRESET._particleSize = 3;
	_ROCKET_THRUSTER_PRESET._emissionRate = 0.002f;
	_ROCKET_THRUSTER_PRESET._startingVelocity = 150;
	_ROCKET_THRUSTER_PRESET._endingVelocity = 0;
	_ROCKET_THRUSTER_PRESET._velVariation = 0.3f;

	_TRON_PRESET = ParticleSettings();
	_TRON_PRESET._particleSize = 10;
	_TRON_PRESET._emissionRate = 0.001f;
	_TRON_PRESET._startingVelocity = 0;
	_TRON_PRESET._endingVelocity = 0;
	_TRON_PRESET._velVariation = 0.0f;
}

ParticleManager::~ParticleManager()
{
}

void ParticleManager::update(float pDT)
{

	Vector2 diff = _position - (*_positionToParentTo + _offset);

	if (diff.Length() > 2)
	{
		if (_velocityList.size() < 3)
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

		_currentVelocity = totalSpeed / _velocityList.size();

		_position = *_positionToParentTo + _offset;
	}


	if (_started == true)
	{
		_timeSinceEmit += pDT;

		while (_timeSinceEmit > _emissionRate)
		{
			SpawnParticle(_currentVelocity.Normalize());
			_timeSinceEmit -= _emissionRate;
		}
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

SDL_Color ParticleManager::GetColour(float pAliveTime)
{
	float totalTime = 0;
	int indexOfColour = 0;
	while (totalTime < pAliveTime && indexOfColour < _colourLerpingList.size())
	{
		totalTime += _colourLerpingList.at(indexOfColour)._durationOfColour;
		indexOfColour++;
	}

	if (indexOfColour == _colourLerpingList.size())
		return{ 0, 0, 0, 0 };

	indexOfColour -= 1;
	if (indexOfColour < 0)
		indexOfColour = 0;

	float percentThroughThisLerp = 1 - ((totalTime - pAliveTime) / _colourLerpingList.at(indexOfColour)._durationOfColour);

	float lerpedR = _colourLerpingList.at(indexOfColour)._colour.r + ((_colourLerpingList.at(indexOfColour + 1)._colour.r - _colourLerpingList.at(indexOfColour)._colour.r)) * percentThroughThisLerp;
	float lerpedG = _colourLerpingList.at(indexOfColour)._colour.g + ((_colourLerpingList.at(indexOfColour + 1)._colour.g - _colourLerpingList.at(indexOfColour)._colour.g)) * percentThroughThisLerp;
	float lerpedB = _colourLerpingList.at(indexOfColour)._colour.b + ((_colourLerpingList.at(indexOfColour + 1)._colour.b - _colourLerpingList.at(indexOfColour)._colour.b)) * percentThroughThisLerp;
	float lerpedA = _colourLerpingList.at(indexOfColour)._colour.a + ((_colourLerpingList.at(indexOfColour + 1)._colour.a - _colourLerpingList.at(indexOfColour)._colour.a)) * percentThroughThisLerp;

	return{ static_cast<Uint8>(lerpedR), static_cast<Uint8>(lerpedG), static_cast<Uint8>(lerpedB), static_cast<Uint8>(lerpedA) };
}


void ParticleManager::render(SDL_Renderer * pRenderer)
{
	for (int i = 0; i < _particleList.size(); i++)
	{
		_particleList[i]->render(pRenderer);
	}
}

#pragma region ButtonFunctions

void ParticleManager::SetUpRocketThruster()
{
	_shapeType = Shape::RocketThruster;
	_textureExampleHolder = nullptr;

	_emissionRate = _ROCKET_THRUSTER_PRESET._emissionRate;
	_startingVelocity = _ROCKET_THRUSTER_PRESET._startingVelocity;
	_endingVelocity = _ROCKET_THRUSTER_PRESET._endingVelocity;
	_particleSize = _ROCKET_THRUSTER_PRESET._particleSize;


	_colourLerpingList.clear();

	ColourLerper firstLerp;
	firstLerp._colour = { 255, 255, 255, 255 };
	firstLerp._durationOfColour = 0.5f;
	_colourLerpingList.push_back(firstLerp);

	ColourLerper secondLerp;
	secondLerp._colour = { 255, 160, 37, 255 };
	secondLerp._durationOfColour = 0.1f;
	_colourLerpingList.push_back(secondLerp);

	ColourLerper thirdLerp;
	thirdLerp._colour = { 100, 100, 100, 0 };
	thirdLerp._durationOfColour = 1.5f;
	_colourLerpingList.push_back(thirdLerp);
}

void ParticleManager::GetDefaultColours()
{
	_colourLerpingList.clear();

	ColourLerper firstLerp;
	firstLerp._colour = { 255, 255, 255, 255 };
	firstLerp._durationOfColour = 0.5f;
	_colourLerpingList.push_back(firstLerp);

	ColourLerper secondLerp;
	secondLerp._colour = { 255, 160, 37, 255 };
	secondLerp._durationOfColour = 0.1f;
	_colourLerpingList.push_back(secondLerp);
}

void ParticleManager::SetupTron()
{
	Game::_tronPos = *Game::_mousePos;
	_positionToParentTo = &Game::_tronPos;
	_shapeType = Shape::Tron;
	_colourLerpingList.clear();
	_emissionRate = _TRON_PRESET._emissionRate;
	_textureExampleHolder = TextureLoader::loadTexture("assets/laser.png", _renderer);
	_startingVelocity = _TRON_PRESET._startingVelocity;
	_endingVelocity = _TRON_PRESET._endingVelocity;
}

void ParticleManager::SetUpFootsteps()
{
	_shapeType = Shape::Footsteps;
	_colourLerpingList.clear();
	_emissionRate = _FOOTSTEPS_PRESET._emissionRate;
	_textureExampleHolder = TextureLoader::loadTexture("assets/footsteps.png", _renderer);
	_startingVelocity = _FOOTSTEPS_PRESET._startingVelocity;
	_endingVelocity = _FOOTSTEPS_PRESET._endingVelocity;
}

void ParticleManager::SetUpTexture()
{
	_shapeType = Shape::NULL_SHAPE;
	_textureExampleHolder = TextureLoader::loadTexture("assets/particle.png", _renderer);
	_emissionRate = 0.002f;
	_startingVelocity = 150;
	_endingVelocity = 20;
	_particleSize = 4;
}

std::string ParticleManager::IncrementShapeType()
{
	_positionToParentTo = Game::_mousePos;

	switch (_shapeType)
	{
	case Shape::Triangle:
		_shapeType = Shape::Square;
		_textureExampleHolder = nullptr;
		if (_colourLerpingList.size() == 0)
			GetDefaultColours();
		return "Square";
	case Shape::Square:
		_shapeType = Shape::Pentagon;
		_textureExampleHolder = nullptr;
		if (_colourLerpingList.size() == 0)
			GetDefaultColours();
		return "Pentagon";
	case Shape::Pentagon:
		_shapeType = Shape::Star;
		_textureExampleHolder = nullptr;
		if (_colourLerpingList.size() == 0)
			GetDefaultColours();
		return "Star";
	case Shape::Star:
		SetUpFootsteps();
		return "Footsteps";
	case Shape::Footsteps:
		SetUpRocketThruster();
		return "RocketThruster";
	case Shape::RocketThruster:
		SetupTron();
		return "Tron";
	case Shape::Tron:
		SetUpTexture();
		return "Texture";
	case Shape::NULL_SHAPE:
		_shapeType = Shape::Triangle;
		_textureExampleHolder = nullptr;
		if (_colourLerpingList.size() == 0)
			GetDefaultColours();
		return "Triangle";
	default:
		return "";
	}
}

std::string ParticleManager::DecrementShapeType()
{
	_positionToParentTo = Game::_mousePos;

	switch (_shapeType)
	{
	case Shape::Triangle:
		SetupTron();
		return "Tron";
	case Shape::Square:
		_shapeType = Shape::Triangle;
		if (_colourLerpingList.size() == 0)
			GetDefaultColours();
		return "Triangle";
	case Shape::Pentagon:
		_shapeType = Shape::Square;
		if (_colourLerpingList.size() == 0)
			GetDefaultColours();
		return "Square";
	case Shape::Star:
		_shapeType = Shape::Pentagon;
		if (_colourLerpingList.size() == 0)
			GetDefaultColours();
		return "Pentagon";
	case Shape::NULL_SHAPE:
		_shapeType = Shape::Star;
		if(_colourLerpingList.size() == 0)
			GetDefaultColours();
		return "Star";
	case Shape::Footsteps:
		SetUpTexture();
		return "Texture";
	case Shape::RocketThruster:
		SetUpFootsteps();
		return "Foot Steps";
	case Shape::Tron:
		SetUpRocketThruster();
		return "RocketThruster";
	default:
		return "";
	}
}

std::string ParticleManager::ChangeEmissionRate(ParticleManager * pManager, bool pIncrement)
{
	if (pIncrement)
		pManager->_emissionRate += 0.005f;
	else if (pManager->_emissionRate > 0.005f)
		pManager->_emissionRate -= 0.005f;
	else
		pManager->_emissionRate = 0.001f;

	return std::to_string(pManager->_emissionRate);
}


std::string ParticleManager::ChangeMinTimeToLive(ParticleManager * pManager, bool pIncrement)
{
	if (pIncrement)
		pManager->_minTTL += 0.2f;
	else
		pManager->_minTTL -= 0.2f;

	return std::to_string(pManager->_minTTL);
}


std::string ParticleManager::ChangeMaxTimeToLive(ParticleManager* pManager, bool pIncrement)
{
	if (pIncrement)
		pManager->_maxTTL += 0.2f;
	else
		pManager->_maxTTL -= 0.2f;

	return std::to_string(pManager->_maxTTL);
}

std::string ParticleManager::ChangeStartingVelocity(ParticleManager* pManager, bool pIncrement)
{
	if (pIncrement)
		pManager->_startingVelocity += 5;
	else
		pManager->_startingVelocity -= 5;

	return std::to_string(int(pManager->_startingVelocity));
}

std::string ParticleManager::ChangeEndingVelocity(ParticleManager* pManager, bool pIncrement)
{
	if (pIncrement)
		pManager->_endingVelocity += 5;
	else
		pManager->_endingVelocity -= 5;

	return std::to_string(int(pManager->_endingVelocity));
}

std::string ParticleManager::ChangeConeAngle(ParticleManager* pManager, bool pIncrement)
{
	if (pIncrement)
		pManager->_particleVelVariation += 0.1f;
	else
		pManager->_particleVelVariation -= 0.1f;

	return std::to_string(pManager->_particleVelVariation);
}

std::string ParticleManager::ChangeParticleType(ParticleManager* pManager, bool pIncrement)
{
	if (pIncrement)
		return pManager->IncrementShapeType();
	else
		return pManager->DecrementShapeType();
}

std::string ParticleManager::ChangeParticleSize(ParticleManager* pManager, bool pIncrement)
{
	if (pIncrement)
		pManager->_particleSize += 0.5f;
	else
		pManager->_particleSize -= 0.5f;

	return std::to_string(pManager->_particleSize);
}

std::string ParticleManager::ChangeStartingRColour(ParticleManager* pManager, bool pIncrement)
{
	if (pIncrement)
		pManager->_colourLerpingList.at(0)._colour.r += 5;
	else
		pManager->_colourLerpingList.at(0)._colour.r -= 5;

	return std::to_string(pManager->_colourLerpingList.at(0)._colour.r);
}
std::string ParticleManager::ChangeStartingGColour(ParticleManager* pManager, bool pIncrement)
{
	if (pIncrement)
		pManager->_colourLerpingList.at(0)._colour.g += 5;
	else
		pManager->_colourLerpingList.at(0)._colour.g -= 5;

	return std::to_string(pManager->_colourLerpingList.at(0)._colour.g);
}
std::string ParticleManager::ChangeStartingBColour(ParticleManager* pManager, bool pIncrement)
{
	if (pIncrement)
		pManager->_colourLerpingList.at(0)._colour.b += 5;
	else
		pManager->_colourLerpingList.at(0)._colour.b -= 5;

	return std::to_string(pManager->_colourLerpingList.at(0)._colour.b);
}
std::string ParticleManager::ChangeStartingAColour(ParticleManager* pManager, bool pIncrement)
{
	if (pIncrement)
		pManager->_colourLerpingList.at(0)._colour.a += 5;
	else
		pManager->_colourLerpingList.at(0)._colour.a -= 5;

	return std::to_string(pManager->_colourLerpingList.at(0)._colour.a);
}

std::string ParticleManager::ChangeEndingRColour(ParticleManager* pManager, bool pIncrement)
{
	if (pIncrement)
		pManager->_colourLerpingList.at(1)._colour.r += 5;
	else
		pManager->_colourLerpingList.at(1)._colour.r -= 5;

	return std::to_string(pManager->_colourLerpingList.at(1)._colour.r);
}
std::string ParticleManager::ChangeEndingGColour(ParticleManager* pManager, bool pIncrement)
{
	if (pIncrement)
		pManager->_colourLerpingList.at(1)._colour.g += 5;
	else
		pManager->_colourLerpingList.at(1)._colour.g -= 5;

	return std::to_string(pManager->_colourLerpingList.at(1)._colour.g);
}
std::string ParticleManager::ChangeEndingBColour(ParticleManager* pManager, bool pIncrement)
{
	if (pIncrement)
		pManager->_colourLerpingList.at(1)._colour.b += 5;
	else
		pManager->_colourLerpingList.at(1)._colour.b -= 5;

	return std::to_string(pManager->_colourLerpingList.at(1)._colour.b);
}
std::string ParticleManager::ChangeEndingAColour(ParticleManager* pManager, bool pIncrement)
{
	if (pIncrement)
		pManager->_colourLerpingList.at(1)._colour.a += 5;
	else
		pManager->_colourLerpingList.at(1)._colour.a -= 5;

	return std::to_string(pManager->_colourLerpingList.at(1)._colour.a);
}

#pragma endregion


void ParticleManager::SpawnParticle(Vector2 pDir)
{
	float randX = (static_cast<float>((rand() % 1000) / 1000.0f) * _particleVelVariation) - _particleVelVariation / 2;
	float randY = (static_cast<float>((rand() % 1000) / 1000.0f) * _particleVelVariation) - _particleVelVariation / 2;

	float xVel = (pDir.x - randX) * _startingVelocity;
	float yVel = (pDir.y - randY) * _startingVelocity;
	float endingXVel = (pDir.x - randX) * _endingVelocity;
	float endingYVel = (pDir.y - randY) * _endingVelocity;

	Vector2 startingVel = Vector2(xVel, yVel);
	Vector2 endingVel = Vector2(endingXVel, endingYVel);

	float timeToLive = (static_cast<float>((rand() % 1000) / 1000.0f) * (_maxTTL - _minTTL)) + _minTTL;
	ParticleObj::ParticleObjSettings settings;

	settings._position = _position;
	settings._size = Vector2(_particleSize, _particleSize);
	settings._startingVelocity = startingVel;
	settings._endingVelocity = endingVel;
	settings._direction = pDir;
	settings._timeToLive = timeToLive;
	settings._texture = _textureExampleHolder;
	settings._shape = nullptr;


	if (_shapeType != Shape::NULL_SHAPE)
	{
		std::vector<Vector2> vertPositions;
		Shape * shape = nullptr;

		float rotSpeed = ((rand() % 1000 / 1000.0f) * _rotationMaxSpeed * 2) - _rotationMaxSpeed;
		float sizeModifier;
		switch (_shapeType)
		{

		case Shape::Triangle:

			sizeModifier = 2.5f;
			vertPositions.push_back(Vector2(0 * _particleSize, -sizeModifier * _particleSize));
			vertPositions.push_back(Vector2(sizeModifier * 2 * _particleSize, -sizeModifier * _particleSize));
			vertPositions.push_back(Vector2(0 * _particleSize, sizeModifier * _particleSize));

			shape = new Shape(_position, vertPositions, Shape::ShapeType::Triangle, _renderer, rotSpeed);

			break;

		case Shape::Square:

			sizeModifier = 1.5f;
			vertPositions.push_back(Vector2(-sizeModifier		* _particleSize, -sizeModifier * _particleSize));
			vertPositions.push_back(Vector2(sizeModifier		* _particleSize, -sizeModifier * _particleSize));
			vertPositions.push_back(Vector2(sizeModifier		* _particleSize, sizeModifier * _particleSize));
			vertPositions.push_back(Vector2(-sizeModifier		* _particleSize, sizeModifier * _particleSize));

			shape = new Shape(_position, vertPositions, Shape::ShapeType::Square, _renderer, rotSpeed);
			break;

		case Shape::Pentagon:

			sizeModifier = 1.5f;
			vertPositions.push_back(Vector2(sizeModifier * 0 * _particleSize, sizeModifier * 0 * -_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * 2 * _particleSize, sizeModifier * 0 * -_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * 2.6f		*		_particleSize, sizeModifier * 2 * -_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * 1 * _particleSize, sizeModifier * 3.1f *		-_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * -0.64f	*		_particleSize, sizeModifier * 2 * -_particleSize));

			shape = new Shape(_position, vertPositions, Shape::ShapeType::Pentagon, _renderer, rotSpeed);
			break;

		case Shape::Star:

			sizeModifier = 0.4f;
			vertPositions.push_back(Vector2(sizeModifier * 6.5		*		_particleSize, sizeModifier * 0 * -_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * 5 * _particleSize, sizeModifier * 5 * -_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * 0 * _particleSize, sizeModifier * 5.5f	*		-_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * 3.5f		*		_particleSize, sizeModifier * 9 * -_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * 2 * _particleSize, sizeModifier * 14 * -_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * 6.5f		*		_particleSize, sizeModifier * 11.5f	*		-_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * 12 * _particleSize, sizeModifier * 14 * -_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * 10.5f	*		_particleSize, sizeModifier * 9 * -_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * 14 * _particleSize, sizeModifier * 5.5f	*		-_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * 9 * _particleSize, sizeModifier * 5 * -_particleSize));


			shape = new Shape(_position, vertPositions, Shape::ShapeType::Star, _renderer, rotSpeed);
			break;

		case Shape::NULL_SHAPE:
			delete shape;
			break;
		case Shape::Footsteps:
			delete shape;
			settings._size = Vector2(_FOOTSTEPS_PRESET._particleSize, _FOOTSTEPS_PRESET._particleSize);
			settings._texture = _textureExampleHolder;
			break;

		case Shape::RocketThruster:

			settings._size = Vector2(_ROCKET_THRUSTER_PRESET._particleSize, _ROCKET_THRUSTER_PRESET._particleSize);

			sizeModifier = 1.5f;
			vertPositions.push_back(Vector2(sizeModifier * 0		* _particleSize, sizeModifier * 0 * -_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * 2		* _particleSize, sizeModifier * 0 * -_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * 2.6f		* _particleSize, sizeModifier * 2 * -_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * 1		* _particleSize, sizeModifier * 3.1f *		-_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * -0.64f	* _particleSize, sizeModifier * 2 * -_particleSize));

			shape = new Shape(_position, vertPositions, Shape::ShapeType::Pentagon, _renderer, rotSpeed);


			break;
		case Shape::Tron:
			delete shape;
			settings._size = Vector2(_TRON_PRESET._particleSize, _TRON_PRESET._particleSize);
			settings._texture = _textureExampleHolder;
			break;

		default:

			break;
		}


		settings._shape = shape;
	}


	_particleList.push_back(new ParticleObj(settings, *this));
}
