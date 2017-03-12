#include "Shape.h"
#include <SDL2_gfxPrimitives.h>
#include <ostream>


Shape::Shape(Vector2 pPos, std::vector<Vector2> pVerts, ShapeType pType, SDL_Renderer * pRenderer, float pRotSpeed)
	:
	_position(pPos),
	_vertices(pVerts),
	_fullScaleVerts(pVerts),
	_angle(0),
	_type(pType),
	_renderer(pRenderer),
	_rotSpeed(pRotSpeed)
{
}

Shape::~Shape()
{
}

void Shape::Draw()
{
	short * x; //filledpolygonRGBA requires to be passed in an array of shorts, so I have one array for each axis.
	short * y;

	int numVerts;

	switch (_type)
	{
	case Triangle:
		numVerts = 3;
		break;

	case Square:
		numVerts = 4;
		break;

	case Pentagon:
		numVerts = 5;
		break;
	case Star:
		numVerts = 10;
		break;

	case NULL_SHAPE:
		numVerts = -1;
		break;

	default:
		numVerts = -1;
		break;
	}

	x = new short[numVerts];
	y = new short[numVerts];

	for (int i = 0; i < numVerts; ++i)
	{
		x[i] = _vertices.at(i).x + _position.x;
		y[i] = _vertices.at(i).y + _position.y;
	}

	filledPolygonRGBA(_renderer,
		x,
		y,
		numVerts,
		_colour.r, _colour.g, _colour.b, _colour.a);

	delete[] x;
	delete[] y;
}

void Shape::Update(Vector2 pVelocity, float pScale, float pDT)
{
	_position = _position + pVelocity;
	Rotate(pDT);
	Scale(pScale);
}

void Shape::Scale(float pValue)
{
	for (int i = 0; i < _vertices.size(); ++i)
	{
		_vertices[i] = _vertices[i].Normalize() * (_fullScaleVerts[i].Length() * pValue);
	}
}

void Shape::Rotate(float pDT)
{
	_angle = _rotSpeed * pDT;

	for (int i = 0; i < _vertices.size(); ++i)
	{
		Vector2 pos = _vertices.at(i);

		_vertices.at(i).x = pos.x * cosf(_angle) - pos.y * sinf(_angle);
		_vertices.at(i).y = pos.x * sinf(_angle) + pos.y * cosf(_angle);
	}
}

void Shape::SetColour(SDL_Color pColour)
{
	_colour = pColour;
}
