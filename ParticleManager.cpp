#pragma once
#include "ParticleManager.h"
#include "FramerateCounter.h"
#include "TextureLoader.h"
#include "Game.h"

ParticleManager::ParticleSettings ParticleManager::_FOOTSTEPS_PRESET;
ParticleManager::ParticleSettings ParticleManager::_ROCKET_THRUSTER_PRESET;
ParticleManager::ParticleSettings ParticleManager::_TRON_PRESET;
ParticleManager::ParticleSettings ParticleManager::_STAR_PRESET;

ParticleManager::ParticleManager(ParticleSettings pSettings, SDL_Renderer * pRenderer)
	:_colourLerpingList(pSettings._coloursToLerp),
	_particleSize(pSettings._particleSize),
	_shapeType(pSettings._shapeType),
	_particleVelVariation(pSettings._velVariation),
	_startingVelocity(pSettings._startingVelocity),
	_endingVelocity(pSettings._endingVelocity),
	_emissionRate(pSettings._emissionRate),
	_texture(pSettings._texture),
	_shouldSystemEmit(true),
	_particleList(std::vector<ParticleObj *>()),
	_positionToParentTo(pSettings._positionToParentTo),
	_currentVelocity(0, 0),
	_timeSinceEmit(0),
	_renderer(pRenderer),
	_rotationSpeed(pSettings._rotationSpeed),
	_emissionShape(pSettings._emissionShape),
	_startingParticleDirection(pSettings._startingDirection),
	_startScale(pSettings._startScale),
	_endScale(pSettings._endScale)
{
	if (_positionToParentTo == nullptr)
		_positionToParentTo = new Vector2(300, 300);

	if (_texture != nullptr)
		_shapeType = Shape::NULL_SHAPE;


	if (_colourLerpingList.size() == 0)
	{
		ColourLerper white = ColourLerper();
		white._colour = { 255,255,255,255 };
		white._durationOfColour = 2;

		_colourLerpingList.push_back(white);

		ColourLerper col2 = ColourLerper();
		col2._colour = { 255,128,0,10 };
		col2._durationOfColour = 2;

		_colourLerpingList.push_back(col2);
	}

	float timeToLiveTotal = 0;

	for (int i = 0; i < _colourLerpingList.size(); ++i)
	{
		timeToLiveTotal += _colourLerpingList.at(i)._durationOfColour;
	}

	_minTTL = timeToLiveTotal - pSettings._timeToLiveVariation;
	_maxTTL = timeToLiveTotal;

	if (_emissionShape == nullptr)
		_emissionShape = new EmissionPoint();
}


void ParticleManager::LoadPresets(SDL_Renderer * pRenderer)
{
	_FOOTSTEPS_PRESET = ParticleSettings();
	_FOOTSTEPS_PRESET._particleSize = 25;
	_FOOTSTEPS_PRESET._emissionRate = 0.424f;
	_FOOTSTEPS_PRESET._startingVelocity = 0;
	_FOOTSTEPS_PRESET._endingVelocity = 0;
	_FOOTSTEPS_PRESET._startScale = 1;
	_FOOTSTEPS_PRESET._endScale = 1;
	_FOOTSTEPS_PRESET._shapeType = Shape::Footsteps;
	_FOOTSTEPS_PRESET._texture = TextureLoader::loadTexture("assets/footsteps.png", pRenderer);

	_ROCKET_THRUSTER_PRESET = ParticleSettings();
	_ROCKET_THRUSTER_PRESET._particleSize = 3;
	_ROCKET_THRUSTER_PRESET._emissionRate = 0.002f;
	_ROCKET_THRUSTER_PRESET._startingVelocity = 150;
	_ROCKET_THRUSTER_PRESET._endingVelocity = 0;
	_ROCKET_THRUSTER_PRESET._velVariation = 0.5f;
	_ROCKET_THRUSTER_PRESET._startScale = 0.25f;
	_ROCKET_THRUSTER_PRESET._endScale = 4;
	_ROCKET_THRUSTER_PRESET._shapeType = Shape::RocketThruster;

	_ROCKET_THRUSTER_PRESET._coloursToLerp.clear();


	_ROCKET_THRUSTER_PRESET._minTTL = 2.0f;
	_ROCKET_THRUSTER_PRESET._maxTTL = 2.1f;



	_TRON_PRESET = ParticleSettings();
	_TRON_PRESET._particleSize = 6;
	_TRON_PRESET._emissionRate = 0.001f;
	_TRON_PRESET._startingVelocity = 0;
	_TRON_PRESET._endingVelocity = 0;
	_TRON_PRESET._velVariation = 0.0f;
	_TRON_PRESET._minTTL = 4;
	_TRON_PRESET._maxTTL = 4;
	_TRON_PRESET._startScale = 1;
	_TRON_PRESET._endScale = 0.3f;
	_TRON_PRESET._shapeType = Shape::Tron;
	_TRON_PRESET._texture = TextureLoader::loadTexture("assets/laser.png", pRenderer);

	_STAR_PRESET = ParticleSettings();
	_STAR_PRESET._particleSize = 2.5f;
	_STAR_PRESET._emissionRate = 0.005f;
	_STAR_PRESET._startingVelocity = 150;
	_STAR_PRESET._endingVelocity = 0;
	_STAR_PRESET._minTTL = 2;
	_STAR_PRESET._maxTTL = 4;
	_STAR_PRESET._velVariation = 1;
	_STAR_PRESET._shapeType = Shape::StarPreset;
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


	if (_shouldSystemEmit == true)
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

		float scale;
		if (_startScale != 1 || _endScale != 1)
		{
			scale = _startScale + (_particleList[i]->_timeAlive / _particleList[i]->_timeToLive) * (_endScale - _startScale);
		}
		else
			scale = 1;


		_particleList[i]->update(pDT, scale);

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
	{
		if (_colourLerpingList.size() > 0)
		{
			return _colourLerpingList.at(_colourLerpingList.size() - 1)._colour;
		}
		else
		{
			return{ 0,0,0,0 };
		}
	}

	indexOfColour -= 1;
	if (indexOfColour < 0)
		indexOfColour = 0;

	float percentThroughThisLerp = 1 - ((totalTime - pAliveTime) / _colourLerpingList.at(indexOfColour)._durationOfColour);

	float lerpedR;
	float lerpedG;
	float lerpedB;
	float lerpedA;

	if(percentThroughThisLerp > 0.95f)
	{
		lerpedR = _colourLerpingList.at(indexOfColour + 1)._colour.r;
		lerpedG = _colourLerpingList.at(indexOfColour + 1)._colour.g;
		lerpedB = _colourLerpingList.at(indexOfColour + 1)._colour.b;
		lerpedA = _colourLerpingList.at(indexOfColour + 1)._colour.a;
	}
	else
	{
		lerpedR = _colourLerpingList.at(indexOfColour)._colour.r + ((_colourLerpingList.at(indexOfColour + 1)._colour.r - _colourLerpingList.at(indexOfColour)._colour.r)) * percentThroughThisLerp;
		lerpedG = _colourLerpingList.at(indexOfColour)._colour.g + ((_colourLerpingList.at(indexOfColour + 1)._colour.g - _colourLerpingList.at(indexOfColour)._colour.g)) * percentThroughThisLerp;
		lerpedB = _colourLerpingList.at(indexOfColour)._colour.b + ((_colourLerpingList.at(indexOfColour + 1)._colour.b - _colourLerpingList.at(indexOfColour)._colour.b)) * percentThroughThisLerp;
		lerpedA = _colourLerpingList.at(indexOfColour)._colour.a + ((_colourLerpingList.at(indexOfColour + 1)._colour.a - _colourLerpingList.at(indexOfColour)._colour.a)) * percentThroughThisLerp;
	}

	return{ static_cast<Uint8>(lerpedR), static_cast<Uint8>(lerpedG), static_cast<Uint8>(lerpedB), static_cast<Uint8>(lerpedA) };
}


void ParticleManager::render()
{
	for (int i = 0; i < _particleList.size(); i++)
	{
		_particleList[i]->render(_renderer);
	}
}

#pragma region ButtonFunctions

void ParticleManager::SetUpRocketThruster()
{
	_shapeType = Shape::RocketThruster;
	_texture = nullptr;

	_emissionRate = _ROCKET_THRUSTER_PRESET._emissionRate;
	_startingVelocity = _ROCKET_THRUSTER_PRESET._startingVelocity;
	_endingVelocity = _ROCKET_THRUSTER_PRESET._endingVelocity;
	_particleSize = _ROCKET_THRUSTER_PRESET._particleSize;
	_endScale = _ROCKET_THRUSTER_PRESET._endScale;
	_startScale = _ROCKET_THRUSTER_PRESET._startScale;

	_colourLerpingList.clear();

	ColourLerper firstLerp;
	firstLerp._colour = { 255, 255, 255, 255 };
	firstLerp._durationOfColour = 0.5f;
	_colourLerpingList.push_back(firstLerp);

	ColourLerper secondLerp;
	secondLerp._colour = { 255, 160, 37, 180 };
	secondLerp._durationOfColour = 0.6f;
	_colourLerpingList.push_back(secondLerp);

	ColourLerper thirdLerp;
	thirdLerp._colour = { 100, 100, 100, 0 };
	thirdLerp._durationOfColour = 1.0f;
	_colourLerpingList.push_back(thirdLerp);

	_minTTL = _ROCKET_THRUSTER_PRESET._minTTL;
	_maxTTL = _ROCKET_THRUSTER_PRESET._maxTTL;
}

void ParticleManager::GetDefaultColours()
{
	_maxTTL = _STAR_PRESET._maxTTL;
	_minTTL = _STAR_PRESET._minTTL;
	_emissionRate = 0.005f;

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
	_texture = TextureLoader::loadTexture("assets/laser.png", _renderer);
	_startingVelocity = _TRON_PRESET._startingVelocity;
	_endingVelocity = _TRON_PRESET._endingVelocity;
	_minTTL = _TRON_PRESET._minTTL;
	_maxTTL = _TRON_PRESET._maxTTL;
	_startScale = _TRON_PRESET._startScale;
	_endScale = _TRON_PRESET._endScale;
}

void ParticleManager::SetUpStarPreset()
{
	_shapeType = Shape::StarPreset;
	_texture = nullptr;
	
	_particleSize = _STAR_PRESET._particleSize;
	_emissionRate = _STAR_PRESET._emissionRate;
	_startingVelocity = _STAR_PRESET._startingVelocity;
	_endingVelocity = _STAR_PRESET._endingVelocity;
	_minTTL = _STAR_PRESET._minTTL;
	_maxTTL = _STAR_PRESET._maxTTL;
	_particleVelVariation = _STAR_PRESET._velVariation;
	_startScale = _STAR_PRESET._startScale;
	_endScale = _STAR_PRESET._endScale;


	_colourLerpingList.clear();

	Uint8 randomCol1 = 235;
	Uint8 randomCol2 = 168;
	Uint8 randomCol3 = 42;

	ColourLerper firstLerp;
	firstLerp._colour = { randomCol1, randomCol2, randomCol3, 255 };
	firstLerp._durationOfColour = 0.75f;
	_colourLerpingList.push_back(firstLerp);

	randomCol1 = 230;
	randomCol2 = 220;
	randomCol3 = 135;

	ColourLerper secondLerp;
	secondLerp._colour = { randomCol1, randomCol2, randomCol3, 255 };
	secondLerp._durationOfColour = 0.75f;
	_colourLerpingList.push_back(secondLerp);

	randomCol1 = 90;
	randomCol2 = 90;
	randomCol3 = 90;

	ColourLerper thirdLerp;
	thirdLerp._colour = { randomCol1, randomCol2, randomCol3, 0 };
	thirdLerp._durationOfColour = 1.5f;
	_colourLerpingList.push_back(thirdLerp);
}

void ParticleManager::SetUpFootsteps()
{
	_shapeType = Shape::Footsteps;
	_colourLerpingList.clear();
	_emissionRate = _FOOTSTEPS_PRESET._emissionRate;
	_texture = TextureLoader::loadTexture("assets/footsteps.png", _renderer);
	_startingVelocity = _FOOTSTEPS_PRESET._startingVelocity;
	_endingVelocity = _FOOTSTEPS_PRESET._endingVelocity;
	_startScale = _FOOTSTEPS_PRESET._startScale;
	_endScale = _FOOTSTEPS_PRESET._endScale;
}

void ParticleManager::SetUpTexture()
{
	_shapeType = Shape::NULL_SHAPE;
	_texture = TextureLoader::loadTexture("assets/particle.png", _renderer);
	_emissionRate = 0.002f;
	_startingVelocity = 150;
	_endingVelocity = 20;
	_particleSize = 4;
	_startScale = 1;
	_endScale = 5;
}

std::string ParticleManager::IncrementShapeType()
{
	_positionToParentTo = Game::_mousePos;

	switch (_shapeType)
	{
	case Shape::Triangle:
		_shapeType = Shape::Square;
		_texture = nullptr;
		if (_colourLerpingList.size() == 0)
			GetDefaultColours();
		if (_emissionRate < 0.005f)
			_emissionRate = 0.005f;
		return "Square";
	case Shape::Square:
		_shapeType = Shape::Pentagon;
		_texture = nullptr;
		if (_colourLerpingList.size() == 0)
			GetDefaultColours();
		if (_emissionRate < 0.005f)
			_emissionRate = 0.005f;
		return "Pentagon";
	case Shape::Pentagon:
		_shapeType = Shape::Star;
		_texture = nullptr;
		if (_colourLerpingList.size() == 0)
			GetDefaultColours();
		if (_emissionRate <0.005f)
			_emissionRate =0.005f;
		return "Star";
	case Shape::Star:
		SetUpFootsteps();
		return "Footsteps";
	case Shape::Footsteps:
		SetUpRocketThruster();
		return "Rocket Thruster";
	case Shape::RocketThruster:
		SetupTron();
		return "Tron";
	case Shape::Tron:
		SetUpStarPreset();
		return "Star Preset";
	case Shape::StarPreset:
		SetUpTexture();
		return "Texture";
	case Shape::NULL_SHAPE:
		_shapeType = Shape::Triangle;
		_texture = nullptr;
		if (_colourLerpingList.size() == 0)
			GetDefaultColours();
		if (_emissionRate < 0.003f)
			_emissionRate = 0.003f;
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
		if (_emissionRate < 0.005f)
			_emissionRate = 0.005f;
		return "Triangle";
	case Shape::Pentagon:
		_shapeType = Shape::Square;
		if (_colourLerpingList.size() == 0)
			GetDefaultColours();
		if (_emissionRate < 0.005f)
			_emissionRate = 0.005f;
		return "Square";
	case Shape::Star:
		_shapeType = Shape::Pentagon;
		if (_colourLerpingList.size() == 0)
			GetDefaultColours();
		if (_emissionRate < 0.005f)
			_emissionRate = 0.005f;
		return "Pentagon";
	case Shape::NULL_SHAPE:
		_shapeType = Shape::Star;
		if (_colourLerpingList.size() == 0)
			GetDefaultColours();
		if (_emissionRate < 0.005f)
			_emissionRate = 0.005f;
		return "Star";
	case Shape::StarPreset:
		SetUpTexture();
		return "Texture";
	case Shape::Footsteps:
		SetUpStarPreset();
		return "Star Preset";
	case Shape::RocketThruster:
		SetUpFootsteps();
		return "Foot Steps";
	case Shape::Tron:
		SetUpRocketThruster();
		return "Rocket Thruster";
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
	

	if(pManager->_emissionRate < 0.005f)
		pManager->_emissionRate = 0.005f;

	return std::to_string(pManager->_emissionRate);
}


std::string ParticleManager::ChangeMinTimeToLive(ParticleManager * pManager, bool pIncrement)
{
	if (pIncrement)
		pManager->_minTTL += 0.2f;
	else
		pManager->_minTTL -= 0.2f;

	if (pManager->_minTTL < 0)
		pManager->_minTTL = 0;

	return std::to_string(pManager->_minTTL);
}


std::string ParticleManager::ChangeMaxTimeToLive(ParticleManager* pManager, bool pIncrement)
{
	if (pIncrement)
		pManager->_maxTTL += 0.2f;
	else
		pManager->_maxTTL -= 0.2f;

	if (pManager->_maxTTL < 0)
		pManager->_maxTTL = 0;

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

	if (pManager->_particleVelVariation < 0)
		pManager->_particleVelVariation = 0;

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
		pManager->_particleSize += 0.02f;
	else
		pManager->_particleSize -= 0.02f;

	if(pManager->_particleSize < 0.02f)
		pManager->_particleSize = 0.02f;

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


	float xVel;
	float yVel;
	float endingXVel;
	float endingYVel;

	if (_startingParticleDirection == nullptr)
	{
		xVel = (pDir.x - randX) * _startingVelocity;
		yVel = (pDir.y - randY) * _startingVelocity;

		endingXVel = (pDir.x - randX) * _endingVelocity;
		endingYVel = (pDir.y - randY) * _endingVelocity;
	}
	else
	{
		xVel = (_startingParticleDirection->x - randX) * _startingVelocity;
		yVel = (_startingParticleDirection->y - randY) * _startingVelocity;

		endingXVel = (_startingParticleDirection->x - randX) * _endingVelocity;
		endingYVel = (_startingParticleDirection->y - randY) * _endingVelocity;
	}

	Vector2 startingVel = Vector2(xVel, yVel);
	Vector2 endingVel = Vector2(endingXVel, endingYVel);

	float timeToLive = (static_cast<float>((rand() % 1000) / 1000.0f) * (_maxTTL - _minTTL)) + _minTTL;
	ParticleObj::ParticleObjSettings settings;





	settings._position = _emissionShape->GetPoint(_position);
	settings._size = Vector2(_particleSize, _particleSize);
	settings._startingVelocity = startingVel;
	settings._endingVelocity = endingVel;
	settings._direction = pDir;
	settings._timeToLive = timeToLive;
	settings._texture = _texture;
	settings._shape = nullptr;


	if (_shapeType != Shape::NULL_SHAPE)
	{
		std::vector<Vector2> vertPositions;
		Shape * shape = nullptr;

		float rotSpeed = ((rand() % 1000 / 1000.0f) * _rotationSpeed * 2) - _rotationSpeed;
		float sizeModifier;
		int randomScale;
		switch (_shapeType)
		{

		case Shape::Triangle:

			sizeModifier = 2.5f;
			vertPositions.push_back(Vector2(0			 * _particleSize,		-sizeModifier * _particleSize));
			vertPositions.push_back(Vector2(sizeModifier * 2 * _particleSize,	-sizeModifier * _particleSize));
			vertPositions.push_back(Vector2(0			 * _particleSize,		 sizeModifier * _particleSize));

			shape = new Shape(settings._position, vertPositions, Shape::ShapeType::Triangle, _renderer, rotSpeed);

			break;

		case Shape::Square:

			sizeModifier = 1.5f;
			vertPositions.push_back(Vector2(-sizeModifier		* _particleSize, -sizeModifier * _particleSize));
			vertPositions.push_back(Vector2(sizeModifier		* _particleSize, -sizeModifier * _particleSize));
			vertPositions.push_back(Vector2(sizeModifier		* _particleSize, sizeModifier  * _particleSize));
			vertPositions.push_back(Vector2(-sizeModifier		* _particleSize, sizeModifier  * _particleSize));

			shape = new Shape(settings._position, vertPositions, Shape::ShapeType::Square, _renderer, rotSpeed);
			break;

		case Shape::Pentagon:

			sizeModifier = 1.5f;
			vertPositions.push_back(Vector2(sizeModifier * 0	 * _particleSize, sizeModifier * 0		* -_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * 2	 * _particleSize, sizeModifier * 0		* -_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * 2.6f	 * _particleSize, sizeModifier * 2		* -_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * 1     * _particleSize, sizeModifier * 3.1f	* -_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * -0.64f* _particleSize, sizeModifier * 2		* -_particleSize));

			shape = new Shape(settings._position, vertPositions, Shape::ShapeType::Pentagon, _renderer, rotSpeed);
			break;

		case Shape::Star:

			sizeModifier = 0.4f;
			vertPositions.push_back(Vector2(sizeModifier * 6.5		* _particleSize, sizeModifier * 0		* -_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * 5		* _particleSize, sizeModifier * 5		* -_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * 0		* _particleSize, sizeModifier * 5.5f	* -_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * 3.5f		* _particleSize, sizeModifier * 9		* -_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * 2		* _particleSize, sizeModifier * 14		* -_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * 6.5f		* _particleSize, sizeModifier * 11.5f	* -_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * 12		* _particleSize, sizeModifier * 14		* -_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * 10.5f	* _particleSize, sizeModifier * 9		* -_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * 14		* _particleSize, sizeModifier * 5.5f	* -_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * 9		* _particleSize, sizeModifier * 5		* -_particleSize));


			shape = new Shape(settings._position, vertPositions, Shape::ShapeType::Star, _renderer, rotSpeed);
			break;

		case Shape::NULL_SHAPE:
			delete shape;
			break;
		case Shape::Footsteps:
			delete shape;
			settings._size = Vector2(_FOOTSTEPS_PRESET._particleSize, _FOOTSTEPS_PRESET._particleSize);
			settings._texture = _texture;
			break;

		case Shape::RocketThruster:

			settings._size = Vector2(_ROCKET_THRUSTER_PRESET._particleSize, _ROCKET_THRUSTER_PRESET._particleSize);

			sizeModifier = 1.5f;
			vertPositions.push_back(Vector2(sizeModifier * 0		* _particleSize, sizeModifier * 0	 * -_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * 2		* _particleSize, sizeModifier * 0	 * -_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * 2.6f		* _particleSize, sizeModifier * 2	 * -_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * 1		* _particleSize, sizeModifier * 3.1f * -_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * -0.64f	* _particleSize, sizeModifier * 2	 * -_particleSize));

			shape = new Shape(settings._position, vertPositions, Shape::ShapeType::Pentagon, _renderer, rotSpeed);


			break;

		case Shape::StarPreset:

			settings._size = Vector2(_STAR_PRESET._particleSize, _STAR_PRESET._particleSize);
			randomScale = rand() % 10;

			sizeModifier = 0.2f + (0.05f * randomScale);
			vertPositions.push_back(Vector2(sizeModifier * 6.5		* _particleSize, sizeModifier		* 0		* -_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * 5		* _particleSize, sizeModifier		* 5		* -_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * 0		* _particleSize, sizeModifier		* 5.5f	* -_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * 3.5f		* _particleSize, sizeModifier		* 9		* -_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * 2		* _particleSize, sizeModifier		* 14	* -_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * 6.5f		* _particleSize, sizeModifier		* 11.5f	* -_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * 12		* _particleSize, sizeModifier		* 14	* -_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * 10.5f	* _particleSize, sizeModifier		* 9		* -_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * 14		* _particleSize, sizeModifier		* 5.5f	* -_particleSize));
			vertPositions.push_back(Vector2(sizeModifier * 9		* _particleSize, sizeModifier		* 5		* -_particleSize));

			shape = new Shape(settings._position, vertPositions, Shape::ShapeType::Star, _renderer, rotSpeed);


			break;
		case Shape::Tron:
			delete shape;
			settings._size = Vector2(_TRON_PRESET._particleSize, _TRON_PRESET._particleSize);
			settings._texture = _texture;
			break;

		default:

			break;
		}


		settings._shape = shape;
	}


	_particleList.push_back(new ParticleObj(settings, *this));
}
