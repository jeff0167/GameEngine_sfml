#pragma once
#include <SFML/Graphics.hpp>
#include "Collider.h"

using namespace sf;
using namespace std;

class BoxCollider : public Collider, public RectangleShape
{
public:
	RectangleShape* rect;

	BoxCollider(RectangleShape _rect) : // somethning non static with transform
		rect(&_rect)
	{
		transform = &_rect; // we technically make an empty transform first then make a new one and give it the ref, which works, but we make an empty transform that is discarded immediately
	};

	virtual std::size_t getPointCount() const override;

	virtual Vector2f getPoint(std::size_t index) const override;

	virtual bool Collision(Collider& _other)
	{
		return false;
	}

private:

};
