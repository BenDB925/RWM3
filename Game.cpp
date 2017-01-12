#include "Game.h"
#include <iostream>
#include <thread>
#include "TextureLoader.h"
#include "TextureParticleManager.h"


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
		m_p_Window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

		if(m_p_Window != 0)
		{
			DEBUG_MSG("Window creation success");
			m_p_Renderer = SDL_CreateRenderer(m_p_Window, -1, 0);
			if(m_p_Renderer != 0)
			{
				DEBUG_MSG("Renderer creation success");
				SDL_SetRenderDrawColor(m_p_Renderer, 0, 0, 0, 255);
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


	TextureParticleManager::ParticleSettings settings = TextureParticleManager::ParticleSettings();
	//position of the particle manager
	_mousePos = new Vector2(0, 0);
	settings._positionToParentTo = _mousePos;
	settings._offsetFromParent = Vector2(0, 0);
	//initial velocity of the particles
	settings._velocity = Vector2(0, 0);
	//the variation of velocity of the particles so that they don't all go in the same direction
	settings._velVariation = Vector2(0.2, 0.2);
	//the min/max time to live
	settings._timeToLive = Vector2(2, 2);
	//the time between particles being emitted
	settings._emissionRate = 0.00001f;

	settings._texture = TextureLoader::loadTexture("assets/particle.png", m_p_Renderer);

	_particleSys = TextureParticleManager(settings);
}

void Game::Update()
{


	_frameCounter.update(m_p_Renderer);
	_particleSys.update(_frameCounter._DT);
}

void Game::Render()
{
	SDL_RenderClear(m_p_Renderer);

	//draw
	_particleSys.render(m_p_Renderer);

	SDL_RenderPresent(m_p_Renderer);
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
	SDL_DestroyWindow(m_p_Window);
	SDL_DestroyRenderer(m_p_Renderer);
	SDL_Quit();
}