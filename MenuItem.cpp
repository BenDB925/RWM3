#include "MenuItem.h"
#include <SDL.h>
#include "Vector2.h"
#include <iomanip>
#include <string>

MenuItem::MenuItem(Vector2 pPos, std::string pVarMess, SDL_Renderer * pRenderer, std::string(*pFunc)(ParticleManager*, bool), ParticleManager * pParticleManager)
	:_particleManager(pParticleManager),
	_func(pFunc),
	_renderer(pRenderer)
{
	_font = TTF_OpenFont("assets/Square.ttf", 2000); //this opens a font style and sets a size
	_varMess = pVarMess;


	if (find(_varMess.begin(), _varMess.end(), '.') != _varMess.end())
	{
		if (_varMess.size() > 5)
		{
			while (_varMess.at(_varMess.size() - 4) != '.')
			{
				_varMess.erase(_varMess.begin() + (_varMess.size() - 1));
			}
		}
	}

	_varSurface = TTF_RenderText_Solid(_font, _varMess.c_str(), { 255, 255, 255 }); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
	_varTexture = SDL_CreateTextureFromSurface(_renderer, _varSurface); //now you can convert it into a texture

	_varRect.x = pPos.x + 15;  //controls the rect's x coordinate 
	_varRect.y = pPos.y + 15; // controls the rect's y coordinte

	_varRect.w = _varMess.length() * 8; // controls the width of the rect
	_varRect.h = 12; // controls the height of the rect

}


MenuItem::~MenuItem()
{
}

void MenuItem::Draw()
{
	SDL_RenderCopy(_renderer, _varTexture, NULL, &_varRect); //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture
}

void MenuItem::ChangeText(bool pIsPositive)
{
	SDL_DestroyTexture(_varTexture);
	SDL_FreeSurface(_varSurface);

	_varMess = _func(_particleManager, pIsPositive);

	if (find(_varMess.begin(), _varMess.end(), '.') != _varMess.end())
	{
		if (_varMess.size() > 5)
		{
			while (_varMess.at(_varMess.size() - 4) != '.')
			{
				_varMess.erase(_varMess.begin() + (_varMess.size() - 1));
			}
		}
	}

	_varRect.w = _varMess.length() * 8; // controls the width of the rect
	_varSurface = TTF_RenderText_Solid(_font, _varMess.c_str(), { 255, 255, 255 }); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
	_varTexture = SDL_CreateTextureFromSurface(_renderer, _varSurface); //now you can convert it into a texture
}
