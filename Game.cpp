#include "Game.h"
#include <iostream>
#include <thread>
#include "TextureLoader.h"
#include "ParticleManager.h"


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

	SDL_Texture* smellText = TextureLoader::loadTexture("assets/stinkyParticle.png", m_p_Renderer);

	Vector2 pos = Vector2(300, 450);
	Vector2 vel = Vector2(0, -10);
	Vector2 velVar = Vector2(3, 3);
	Vector2 minMaxTTL = Vector2(2.5f, 15);

	_particleSys = ParticleManager(pos, vel, velVar, 2000, minMaxTTL, smellText);
}

void Game::Update()
{
	_frameCounter.update(m_p_Renderer);

	_particleSys.update();
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
				int mouseX;
				int mouseY;
				SDL_GetMouseState(&mouseX, &mouseY);
				_particleSys.followMouse(mouseX, mouseY);
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