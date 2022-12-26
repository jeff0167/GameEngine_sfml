#pragma once
#include <SFML/Graphics.hpp>
#include "Component.h"

using namespace sf;
using namespace std;

class Rigidbody : public Component
{
public:
	Vector2f velocity;
	Vector2f rotationalVelocity;
	Transformable* transform;

	Rigidbody() {}
	double Magnitude();
	void AddForce(float force);

private:

};
