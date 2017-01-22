#include "MenuManager.h"
#include "Vector2.h"
#include "ParticleManager.h"

MenuManager * MenuManager::_instance;

MenuManager::MenuManager(): 
	_renderer(nullptr),
	_selectedMenuItem(0)
{

}


MenuManager::~MenuManager()
{
}

void MenuManager::AddItem(Vector2 pPos, std::string pVarMessage, std::string(*pFunc)(ParticleManager*, bool), ParticleManager * pManager)
{
	_menuList.push_back(new MenuItem(pPos, pVarMessage, _renderer, pFunc, pManager));
}

void MenuManager::Init(SDL_Renderer* pRenderer)
{
	_renderer = pRenderer;


	Vector2 position = Vector2(30, 40);

	float size = 10;
	std::vector<Vector2> vertPositions;
	vertPositions.push_back(Vector2(1.5f * size, 0 * size));
	vertPositions.push_back(Vector2(0 * size, 1.5f * size));
	vertPositions.push_back(Vector2(3 * size, 1.5f * size));
	_instance->_selectedShape = new Shape(position, vertPositions, Shape::ShapeType::Triangle, _renderer, 0);
	_instance->_selectedShape->SetColour({ 255,255,255,255 });
}

void MenuManager::Draw()
{
	for (int i = 0; i < _menuList.size(); ++i)
	{
		_menuList.at(i)->Draw();
	}

	_selectedShape->Draw();
}

void MenuManager::HandleInput(Input pInput)
{
	switch (pInput)
	{
	case Left: 
		if (_selectedMenuItem > 0)
		{
			_selectedMenuItem--;
			_selectedShape->_position = Vector2(_menuList.at(_selectedMenuItem)->_varRect.x, 40);
		}
		break;
	case Right:
		if (_selectedMenuItem < _menuList.size() - 1)
		{
			_selectedMenuItem++;
			_selectedShape->_position = Vector2(_menuList.at(_selectedMenuItem)->_varRect.x, 40);
		}
		break;
	case Up: 
		_menuList.at(_selectedMenuItem)->ChangeText(true);
		break;
	case Down:
		_menuList.at(_selectedMenuItem)->ChangeText(false);
		break;
	default: ;
	}
}


MenuManager* MenuManager::Instance()
{
	if (_instance == nullptr)
		_instance = new MenuManager();

	return _instance;
}
