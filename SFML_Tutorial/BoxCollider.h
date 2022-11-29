#pragma once
#include <SFML/Graphics.hpp>
#include "Collider.h"

using namespace sf;
using namespace std;

class BoxCollider : public Collider
{
public:
	float size = 1;
	BoxCollider() {};

	virtual std::size_t getPointCount() const override;

	virtual Vector2f getPoint(std::size_t index) const override;

	virtual bool Collision(Collider& _other)
	{
		return false;
	}

private:

};
