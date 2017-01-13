#pragma once
#include "Vector2.h"
#include <vector>
#include <SDL.h>

class Shape
{
public:

	enum ShapeType
	{
		Triangle,
		Square,
		Pentagon,
		Star,
		NULL_SHAPE
	};

	//relative to position
	Shape(Vector2 pPos, std::vector<Vector2> pVerts, ShapeType pType, SDL_Renderer* pRenderer, SDL_Color pColour, float pRotSpeed);
	~Shape();
	void Draw();
	void Update(Vector2 _velocity, float pDT);
	void Rotate(float pDT);

	Vector2 _position;
	std::vector<Vector2> _vertices;
	float _angle;
	ShapeType _type;
	SDL_Renderer * _renderer;
	SDL_Color _colour;
	float _rotSpeed;
};

