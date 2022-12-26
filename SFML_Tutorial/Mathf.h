#pragma once
#include <SFML/Graphics.hpp>
#include "BoxCollider.h"
#include "CircleCollider.h"
#include "Collider.h"
#include "Physics.h"
#include "Debug.h"

using namespace sf;
using namespace std;

class Mathf
{
public:
	static Vector2f Zero()
	{
		return Vector2f(0, 0);
	}

	static double Magnitude(Vector2f& velocity)
	{
		return sqrt(pow(velocity.x, 2) + pow(velocity.y, 2));
	}

	static Vector2f Normalize(Vector2f& source)
	{
		double length = Magnitude(source);

		if (source.x == 0 && source.y == 0) return source;

		source.x = source.x / length;
		source.y = source.y / length;
		return source;
	}

	static Vector2f DirectionVector(Vector2f start, Vector2f end)
	{
		return Vector2f(start.x - end.x, start.y - end.y);
	}

};