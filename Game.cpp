#include "Game.h"
#include <iostream>
#include <thread>
#include "TextureLoader.h"
#include "ParticleManager.h"
#include "Debug.h"
#include <SDL_ttf.h>
#include "MenuManager.h"

using namespace std;
Vector2 * Game::_mousePos;
Vector2 Game::_tronPos;


Game::Game() : m_running(false), _frameCounter()
{
	_mousePos = new Vector2(300, 300);
}

Game::~Game()
{
}

bool Game::Initialize(const char* title, int xpos, int ypos, int width, int height, int flags)
{
	if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		DEBUG_MSG("SDL Init success");

		_window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		_surface = SDL_GetWindowSurface(_window);

		if(_window != nullptr)
		{
			DEBUG_MSG("Window creation success");
			_renderer = SDL_CreateRenderer(_window, -1, 0);
			if(_renderer != nullptr)
			{
				DEBUG_MSG("Renderer creation success");
				SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
			}
			else
			{
				DEBUG_MSG("Renderer init fail");
				return false;
			}
		}
		else
		{
			DEBUG_MSG("Window init fail");
			return false;
		}
	}
	else
	{
		DEBUG_MSG("SDL init fail");
		return false;
	}
	m_running = true;

	return true;
}

void Game::LoadContent()
{
	DEBUG_MSG("Loading Content");

	//Initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		return;
	}

	_menuTex = TextureLoader::loadTexture("assets/menu.png", _renderer);

	ParticleManager::LoadPresets(_renderer);

	ParticleManager::ParticleSettings settings = ParticleManager::ParticleSettings();
	//position of the particle manager
	settings._positionToParentTo = _mousePos;
	//initial velocity of the particles
	settings._startingVelocity = 0;
	//final velocity of the particles
	settings._endingVelocity = 0;
	//the time between particles being emitted
	settings._emissionRate = 0.005f;

	//the variation in seconds of the time to live variables
	settings._timeToLiveVariation = 2;

	//the rotation speed of the particles
	settings._rotationSpeed = 10;

	//the shape of the emitter
	settings._emissionShape = new ParticleManager::EmissionPoint();


	//the initial scale of the particles
	settings._startScale = 0.3f;

	//the final scale of the particles
	settings._endScale = 3;

	_particleSys = ParticleManager(settings, _renderer);
	

	MenuManager::Instance()->Init(_renderer);
	MenuManager::Instance()->AddItem(Vector2(40, 0),  "Pentagon",												   &ParticleManager::ChangeParticleType,	&_particleSys);
	MenuManager::Instance()->AddItem(Vector2(175, 0),  to_string(_particleSys._particleSize),					   &ParticleManager::ChangeParticleSize,	&_particleSys);
	MenuManager::Instance()->AddItem(Vector2(280, 0),  to_string(settings._emissionRate),						   &ParticleManager::ChangeEmissionRate,	&_particleSys);
	MenuManager::Instance()->AddItem(Vector2(410, 0),  to_string(_particleSys._minTTL),							   &ParticleManager::ChangeMinTimeToLive,	&_particleSys);
	MenuManager::Instance()->AddItem(Vector2(515, 0),  to_string(_particleSys._maxTTL),							   &ParticleManager::ChangeMaxTimeToLive,	&_particleSys);
	MenuManager::Instance()->AddItem(Vector2(645, 0),  to_string(settings._startingVelocity),					   &ParticleManager::ChangeStartingVelocity,&_particleSys);
	MenuManager::Instance()->AddItem(Vector2(780, 0),  to_string(settings._endingVelocity),						   &ParticleManager::ChangeEndingVelocity,	&_particleSys);
	MenuManager::Instance()->AddItem(Vector2(865, 0),  to_string(settings._velVariation),						   &ParticleManager::ChangeConeAngle,		&_particleSys);
	MenuManager::Instance()->AddItem(Vector2(945, 0),  to_string(_particleSys._colourLerpingList.at(0)._colour.r), &ParticleManager::ChangeStartingRColour, &_particleSys);
	MenuManager::Instance()->AddItem(Vector2(1035, 0), to_string(_particleSys._colourLerpingList.at(0)._colour.g), &ParticleManager::ChangeStartingGColour, &_particleSys);
	MenuManager::Instance()->AddItem(Vector2(1080, 0), to_string(_particleSys._colourLerpingList.at(0)._colour.b), &ParticleManager::ChangeStartingBColour, &_particleSys);
	MenuManager::Instance()->AddItem(Vector2(1130, 0), to_string(_particleSys._colourLerpingList.at(0)._colour.a), &ParticleManager::ChangeStartingAColour, &_particleSys);
	MenuManager::Instance()->AddItem(Vector2(1220, 0), to_string(_particleSys._colourLerpingList.at(1)._colour.r), &ParticleManager::ChangeEndingRColour,   &_particleSys);
	MenuManager::Instance()->AddItem(Vector2(1290, 0), to_string(_particleSys._colourLerpingList.at(1)._colour.g), &ParticleManager::ChangeEndingGColour,   &_particleSys);
	MenuManager::Instance()->AddItem(Vector2(1345, 0), to_string(_particleSys._colourLerpingList.at(1)._colour.b), &ParticleManager::ChangeEndingBColour,   &_particleSys);
	MenuManager::Instance()->AddItem(Vector2(1400, 0), to_string(_particleSys._colourLerpingList.at(1)._colour.a), &ParticleManager::ChangeEndingAColour,   &_particleSys);

	_emitorShapes.push_back(MenuItem(Vector2(700, 850), "Point", _renderer, &ParticleManager::ChangeParticleType, &_particleSys));
	_emitorShapes.push_back(MenuItem(Vector2(700, 850), "Rectangle", _renderer, &ParticleManager::ChangeParticleType, &_particleSys));
	_emitorShapes.push_back(MenuItem(Vector2(700, 850), "Circle", _renderer, &ParticleManager::ChangeParticleType, &_particleSys));
	_emitorShapes.push_back(MenuItem(Vector2(700, 850), "Ring", _renderer, &ParticleManager::ChangeParticleType, &_particleSys));
	_emitorShapes.push_back(MenuItem(Vector2(700, 850), "Triangle", _renderer, &ParticleManager::ChangeParticleType, &_particleSys));
}

void Game::Update()
{
	_frameCounter.update(_renderer);

	_particleSys.update(_frameCounter._DT);

	_timeSinceDirChange += _frameCounter._DT;

	if(_particleSys._shapeType == Shape::Tron)
	{
		if (_timeSinceDirChange > _TIME_BEFORE_DIR_CHANGE)
		{
			_timeSinceDirChange = 0;
			float xDist = _mousePos->x - _tronPos.x;
			float yDist = _mousePos->y - _tronPos.y;

			if(abs(xDist) > abs(yDist))
			{
				if (xDist > 0)
					_direction = Right;
				else
					_direction = Left;
			}
			else
			{
				if (yDist < 0)
					_direction = Down;
				else
					_direction = Up;
			}

			_particleSys._velocityList.clear();
		}

		switch (_direction)
		{
		case Up:
			_tronPos.y += _TRON_VEL;
			break;
		case Down:
			_tronPos.y -= _TRON_VEL;
			break;
		case Left:
			_tronPos.x -= _TRON_VEL;
			break;
		case Right:
			_tronPos.x += _TRON_VEL;
			break;
		default: break;
		}
	}

}

void Game::Render()
{
	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
	SDL_RenderClear(_renderer);


	//draw
	_particleSys.render();
	SDL_Rect menuRect{ 0, 0, 1440, 15 };
	MenuManager::Instance()->Draw();

	_emitorShapes[_emitorShapeIndex].Draw();


	SDL_RenderCopy(_renderer, _menuTex, nullptr, &menuRect);
	SDL_RenderPresent(_renderer);
}

void Game::FindParticleSystemShape()
{
	if (_particleSys._emissionShape != nullptr)
		delete _particleSys._emissionShape;

	switch (_emitorShapeIndex)
	{
	case 0:
		_particleSys._emissionShape = new ParticleManager::EmissionPoint();
		break;
	case 1:
		_particleSys._emissionShape = new ParticleManager::EmissionRect(450, 100);
		break;
	case 2:
		_particleSys._emissionShape = new ParticleManager::EmissionCircle(100);
		break;
	case 3:
		_particleSys._emissionShape = new ParticleManager::EmissionRing(100, 150);
		break;
	case 4:
		_particleSys._emissionShape = new ParticleManager::EmissionTriangle(Vector2(0.2f, 0), Vector2(0.5f, -0.5f), Vector2(0.7f, 0), 200);
		break;
	}
}

void Game::HandleEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{


		int mouseX;
		int mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);

		_mousePos->x = mouseX - 2;
		_mousePos->y = mouseY + 5;
			
		switch(event.type)
		{
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					m_running = false;
					break;

				case SDLK_LEFT:
					MenuManager::Instance()->HandleInput(MenuManager::Input::Left);
					break;
				case SDLK_RIGHT:
					MenuManager::Instance()->HandleInput(MenuManager::Input::Right);
					break;
				case SDLK_UP:
					MenuManager::Instance()->HandleInput(MenuManager::Input::Up);
					break;
				case SDLK_DOWN:
					MenuManager::Instance()->HandleInput(MenuManager::Input::Down);
					break;
				case SDLK_a:
					_emitorShapeIndex--;
					if (_emitorShapeIndex < 0)
						_emitorShapeIndex = _EMITORSHAPECOUNT - 1;
					FindParticleSystemShape();
					break;
				case SDLK_d:
					_emitorShapeIndex++;
					if (_emitorShapeIndex >= _EMITORSHAPECOUNT)
						_emitorShapeIndex = 0;
					FindParticleSystemShape();
					break;

				case SDLK_SPACE:
					_particleSys.TurnOnOff();
					break;
				default:
					break;
				}
			case SDL_MOUSEBUTTONDOWN:
				break;

			case SDL_QUIT:
				m_running = false;
				break;
		default: break;
		}

	}
}

bool Game::IsRunning()
{
	return m_running;
}

void Game::UnloadContent()
{
	DEBUG_MSG("Unloading Content");
}

void Game::CleanUp()
{
	DEBUG_MSG("Cleaning Up");
	SDL_DestroyWindow(_window);
	SDL_DestroyRenderer(_renderer);
	SDL_Quit();
}