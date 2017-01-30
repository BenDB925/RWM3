
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <iostream>
#include "Game.h"
#include <thread>
#include <string>
#include "Debug.h"

#pragma comment(lib,"SDL2.lib") 
#pragma comment(lib, "SDL2_image.lib")
#pragma comment(lib,"SDL2_gfx.dll") 
#pragma comment(lib, "SDL2_gfx.lib")
#pragma comment(lib,"SDL2_ttf.lib") 
#pragma comment(lib, "SDL2_ttf.dll")

using namespace std;

class Process
{
public:
	Process() {}
	Process(const Game& game) : m_Game(game)
	{

	}
	~Process() {};
	void run()
	{
		while (m_Game.IsRunning())
		{
			//DEBUG_MSG("Thread Running");
		}
	}
private:
	Game m_Game;
};

int main(int argc, char** argv) {

	DEBUG_MSG("Game Object Created");

	Game* game = new Game();

	//Adjust screen positions as needed
	game->Initialize("Trail Renderer Demo", 300, 100, 1440, 900, SDL_WINDOW_INPUT_FOCUS);
	DEBUG_MSG("Game Initialised");

	game->LoadContent();

	thread t1(&Process::run, Process((*(game)))); //Passing references
	t1.detach(); //detaches from SDL mainline

	DEBUG_MSG("Game Loop Starting......");
	while (game->IsRunning())
	{
		game->HandleEvents();
		game->Update();
		game->Render();
	}

	DEBUG_MSG("Calling Cleanup");
	game->CleanUp();
	game->UnloadContent();

	return 0;
}

