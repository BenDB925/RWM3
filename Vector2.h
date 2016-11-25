#pragma once
#include <math.h>

class Vector2
{
public:
	Vector2(float pX = 0, float pY = 0) : x(pX), y(pY) { }
	~Vector2(){}

	float Vector2::Length() const
	{
		return sqrt(x * x + y * y);
	}

	Vector2 Vector2::Normalize() const
	{
		Vector2 vector;
		float length = this->Length();

		if (length != 0) {
			vector.x = x / length;
			vector.y = y / length;
		}

		return vector;
	}

	float x;
	float y;
};

