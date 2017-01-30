#define SDL_MAIN_HANDLED
#include "ParticleManager.h"


int main(int argc, char** argv) {

	SDL_Renderer * _renderer = nullptr;

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		SDL_Window *_window = SDL_CreateWindow("testScene", 0, 100, 1440, 900, NULL);

		if (_window != nullptr)
		{
			_renderer = SDL_CreateRenderer(_window, -1, 0);		}
	}

	Vector2 * position = new Vector2(300, 200);

	
	ParticleManager::LoadPresets(_renderer);
	ParticleManager::ParticleSettings settings = ParticleManager::_ROCKET_THRUSTER_PRESET;
	settings._positionToParentTo = position;

	ParticleManager _particleSys = ParticleManager(settings, _renderer);

	while(true)	{
		_particleSys.update(0.01f); // replace this 0.01f with delta time
		position->x += 0.25f;

		SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
		SDL_RenderClear(_renderer);		_particleSys.render();
		SDL_RenderPresent(_renderer);	}


	return 0;
}
