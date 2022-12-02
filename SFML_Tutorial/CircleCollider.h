#pragma once
#include <SFML/Graphics.hpp>
#include "Collider.h"

using namespace sf;
using namespace std;

class CircleCollider : public Collider
{
public:
	CircleShape* rect;

	CircleCollider(CircleShape& _rect) :
		rect(&_rect)
	{
		s = &_rect;
	}

	virtual bool Collision(Collider& _other)
	{
		return false;
	}

private:

};
