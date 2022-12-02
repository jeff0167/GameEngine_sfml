#pragma once
#include <SFML/Graphics.hpp>
#include "Collider.h"

using namespace sf;
using namespace std;

class BoxCollider : public Collider
{
public:
	RectangleShape* rect;

	BoxCollider(RectangleShape& _rect) : // somethning non static with transform
		rect(&_rect)
	{
		s = &_rect;
	};

	virtual bool Collision(Collider& _other)
	{
		return false;
	}

private:

};
