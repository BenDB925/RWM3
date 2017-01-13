#include "Shape.h"
#include <SDL2_gfxPrimitives.h>


Shape::Shape(Vector2 pPos, std::vector<Vector2> pVerts, ShapeType pType, SDL_Renderer * pRenderer, SDL_Color pColour)
	:_angle(0),
	_position(pPos),
	_vertices(pVerts),
	_type(pType),
	_renderer(pRenderer),
	_colour(pColour)
{
}

Shape::~Shape()
{
}

void Shape::Draw()
{
	switch (_type)
	{
	case Triangle:

		filledTrigonRGBA(_renderer,
			_vertices.at(0).x, _vertices.at(0).y,
			_vertices.at(1).x, _vertices.at(1).y,
			_vertices.at(2).x, _vertices.at(2).y,
			_colour.r, _colour.g, _colour.b, _colour.a);
		break;

	default:
		break;
	}
}

void Shape::Update(Vector2 _velocity)
{
	for (int i = 0; i < _vertices.size(); ++i)
	{
		Vector2 pos = _vertices.at(i);

		_vertices.at(i) = pos + _velocity;
	}
}

void Shape::Rotate(float pAngle)
{
	_angle += pAngle;

	for (int i = 0; i < _vertices.size(); ++i)
	{
		Vector2 pos = _vertices.at(i);

		_vertices.at(i).x = pos.x * cosf(_angle) - pos.y * sinf(_angle);
		_vertices.at(i).y = pos.x * sinf(_angle) + pos.y * cosf(_angle);
	}
}
