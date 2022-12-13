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
	int Member;
	GameObject* gameObject;

	Component() {};
	virtual void AbstractNullFunc() 
	{
	}

	bool operator == (const Component& Ref) const
	{
		return(this->Member == Ref.GetMember());
	}

	const int GetMember() const
	{
		return(this->Member);
	}

	string ToString() 
	{
		return "Im a component";
	}

private:

};
