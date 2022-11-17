#pragma once
#include <SFML/Graphics.hpp>
#include "Collider.h"
#include "CircleCollider.h"
#include <iostream>

using namespace sf;
using namespace std;

class Mathf
{
public:
	static Vector2f normalize(const Vector2f& source)
	{
		float length = sqrt((source.x * source.x) + (source.y * source.y));

		if (source.x != 0 && source.y != 0) {
			return Vector2f(source.x / length, source.y / length);
		}
		else
			return source;
	}

private:

};