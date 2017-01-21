#include "Game.h"
#include <iostream>
#include <thread>
#include "TextureLoader.h"
#include "ParticleManager.h"
#include <SDL2_gfxPrimitives.h>
#include "Debug.h"
#include "MenuItem.h"
#include <SDL_ttf.h>
#include "MenuManager.h"


using namespace std;

Game::Game() : m_running(false), _frameCounter()
{
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

		if(_window != 0)
		{
			DEBUG_MSG("Window creation success");
			_renderer = SDL_CreateRenderer(_window, -1, 0);
			if(_renderer != 0)
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


class A {
public:
	int f();
	int (A::*x)(); // <- declare by saying what class it is a pointer to
};

int A::f() {
	return 1;
}


void Game::LoadContent()
{
	DEBUG_MSG("Loading Content");

	//Initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		return;
	}

	A a;
	a.x = &A::f;

	ParticleManager::ParticleSettings settings = ParticleManager::ParticleSettings();
	//position of the particle manager
	_mousePos = new Vector2(0, 0);
	settings._positionToParentTo = _mousePos;
	settings._offsetFromParent = Vector2(0, 0);
	//initial velocity of the particles
	settings._velocity = Vector2(0, 0);
	//the variation of velocity of the particles so that they don't all go in the same direction
	settings._velVariation = Vector2(0.2, 0.2);
	//the time between particles being emitted
	settings._emissionRate = 0.001f;

	settings._timeToLiveVariation = 2;

	settings._texture = TextureLoader::loadTexture("assets/particle.png", _renderer);
	settings._shapeType =  Shape::ShapeType::Triangle;

	settings._rotationMaxSpeed = 0.05f;


	ParticleManager::ColourLerper firstLerp;
	firstLerp._colour = { 255, 0, 0, 255 };
	firstLerp._durationOfColour = 1.5f;
	settings._coloursToLerp.push_back(firstLerp);

	ParticleManager::ColourLerper secondLerp;
	secondLerp._colour = { 0, 0, 255, 150 };
	secondLerp._durationOfColour = 1.0f;
	settings._coloursToLerp.push_back(secondLerp);

	ParticleManager::ColourLerper thirdLerp;
	thirdLerp._colour = { 0, 255, 0, 150 };
	thirdLerp._durationOfColour = 0.5f;
	settings._coloursToLerp.push_back(thirdLerp);

	ParticleManager::ColourLerper fourthL;
	fourthL._colour = { 255, 255, 255, 255 };
	fourthL._durationOfColour = 1.5f;
	settings._coloursToLerp.push_back(fourthL);


	ParticleManager::ColourLerper fiveL;
	fiveL._colour = { 0, 255, 255, 150 };
	fiveL._durationOfColour = 2.5f;
	settings._coloursToLerp.push_back(fiveL);

	ParticleManager::ColourLerper sixL;
	sixL._colour = { 173, 130, 54, 0 };
	sixL._durationOfColour = 4.5f;
	settings._coloursToLerp.push_back(sixL);

	_particleSys = ParticleManager(settings, _renderer);


	MenuManager::Instance()->Init(_renderer);
	MenuManager::Instance()->AddItem(Vector2(0, 0),	  _particleSys.GetEmissionRate(), 0.01f, "Emission Rate", &ParticleManager::ChangeEmissionRate, &_particleSys);
	MenuManager::Instance()->AddItem(Vector2(120, 0), _particleSys.GetMinTimeToLive(), 0.5f, "Min TTL", &ParticleManager::ChangeMinTimeToLive, &_particleSys);
	MenuManager::Instance()->AddItem(Vector2(200, 0), _particleSys.GetMaxTimeToLive(), 0.5f, "Max TTL", &ParticleManager::ChangeMaxTimeToLive, &_particleSys);
}

void Game::Update()
{
	_frameCounter.update(_renderer);
	_particleSys.update(_frameCounter._DT);
}

void Game::Render()
{
	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
	SDL_RenderClear(_renderer);

	MenuManager::Instance()->Draw();

	//draw
	_particleSys.render(_renderer);

	SDL_RenderPresent(_renderer);
}


void Game::HandleEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{


		int mouseX;
		int mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);
		_mousePos->x = mouseX;
		_mousePos->y = mouseY;

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
				}
			case SDL_MOUSEBUTTONDOWN:
				break;

			case SDL_QUIT:
				m_running = false;
				break;
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