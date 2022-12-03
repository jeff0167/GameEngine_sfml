#pragma once
#include <SFML/Graphics.hpp>
#include "Collider.h"

using namespace sf;
using namespace std;

class CircleCollider : public Collider
{
public:
	CircleShape* rect;
	CircleCollider() {};
	CircleCollider(CircleShape& _rect, Vector2f pos, Color color = Color::White) :
		rect(&_rect)
	{
		rect->setPosition(pos);
		rect->setOrigin(rect->getRadius(), rect->getRadius());
		rect->setFillColor(color);
		shape = &_rect;
	};

	virtual bool Collision(Collider& _other)
	{
		return false;
	}

private:

};
