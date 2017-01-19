#pragma once
#include <cmath>
#include "Vector2i.h"

class Vector2
{
public:

	float x;
	float y;

	Vector2(float pX = 0, float pY = 0) : x(pX), y(pY) { }
	~Vector2() {}

	float Vector2::Length() const
	{
		return sqrt(x * x + y * y);
	}

	Vector2 Vector2::Normalize() const
	{
		Vector2 vector;
		float length = this->Length();

		if (length != 0)
		{
			vector.x = x / length;
			vector.y = y / length;
		}

		return vector;
	}

	Vector2 operator * (const float val) const
	{
		return Vector2(x * val, y * val);
	}

	Vector2 operator / (const float val) const
	{
		return Vector2(x / val, y / val);
	}

	Vector2 operator - (const Vector2 pVec) const
	{
		return Vector2(x - pVec.x, y - pVec.y);
	}

	Vector2 operator + (const Vector2 pVec) const
	{
		return Vector2(x + pVec.x, y + pVec.y);
	}

	Vector2 operator + (const Vector2i pVec) const
	{
		return Vector2(x + pVec.x, y + pVec.y);
	}

};

