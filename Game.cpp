#include "Game.h"
#include <iostream>
#include <thread>
#include "TextureLoader.h"
#include "ParticleMan.h"
#include <SDL2_gfxPrimitives.h>
#include "Debug.h"


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



void Game::LoadContent()
{
	DEBUG_MSG("Loading Content");


	ParticleMan::ParticleSettings settings = ParticleMan::ParticleSettings();
	//position of the particle manager
	_mousePos = new Vector2(0, 0);
	settings._positionToParentTo = _mousePos;
	settings._offsetFromParent = Vector2(0, 0);
	//initial velocity of the particles
	settings._velocity = Vector2(0, 0);
	//the variation of velocity of the particles so that they don't all go in the same direction
	settings._velVariation = Vector2(0.2, 0.2);
	//the min/max time to live
	settings._timeToLive = Vector2(2, 2.5f);
	//the time between particles being emitted
	settings._emissionRate = 0.00001f;

	settings._texture = TextureLoader::loadTexture("assets/particle.png", _renderer);

	_particleSys = ParticleMan(settings);
}

void Game::Update()
{


	_frameCounter.update(_renderer);
	_particleSys.update(_frameCounter._DT);
}

void Game::Render()
{
	SDL_RenderClear(_renderer);

	//int result1 = filledCircleColor(_renderer, 0, 0, 5000, 0xFF0000FF);

	filledTrigonColor(_renderer,
		0, 0,
		50, 50,
		100, 150,
		0xFF0000FF);	
	
	filledTrigonColor(_renderer,
			130, 160,
			540, 50,
			100, 350,
			0xFF0000FF);


	filledTrigonRGBA(_renderer, 
		300, 300, 
		400, 400, 
		380, 360, 
		255, 0, 255, 255);

	filledPieRGBA(_renderer,
		500, 500,
		200,
		30, 270,
		0, 0, 200, 255);


	filledCircleRGBA(_renderer,
		500, 500,
		200,
		0, 0, 200, 255);

	//filledTrigonColor(_renderer,
	//	200, 
	//	50,
	//	100,
	//	120,
	//	140,
	//	160,
	//	0xa1f442);


	filledCircleColor(_renderer,
		200,
		50,
		100,
		0x00000);

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