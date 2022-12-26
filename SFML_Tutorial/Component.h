#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"

using namespace sf;
using namespace std;

class ParticleSystemUpdate
{
public:
	virtual void Update() = 0;

	virtual void SetEmitterTransform(Transformable& transform) = 0;
};

class Component
{
public:
	GameObject* gameObject;

	Component() {};
	virtual void AbstractNullFunc() 
	{
	}

	virtual string ToString() 
	{
		string className = typeid(*this).name();
		return "Im a " + className;
	}

private:

};
