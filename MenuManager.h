#pragma once
#include <vector>
#include "MenuItem.h"
#include "Shape.h"

class MenuManager
{
public:

	enum Input
	{
		Left,
		Right,
		Up,
		Down
	};

	MenuManager();
	~MenuManager();

	void AddItem(Vector2 pPos, std::string pMessage, std::string pVarMessage, std::string (*pFunc)(ParticleManager*, bool), ParticleManager* pManager);
	void Init(SDL_Renderer * pRenderer);

	void Draw();
	void HandleInput(Input pInput);

	static MenuManager * Instance();

private:
	static MenuManager * _instance;
	std::vector<MenuItem *> _menuList;
	SDL_Renderer * _renderer;
	int _selectedMenuItem;
	Shape * _selectedShape;
};

