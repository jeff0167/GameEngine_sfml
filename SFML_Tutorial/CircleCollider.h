#pragma once
#include <SFML/Graphics.hpp>
#include "Component.h"
#include "Collider.h"

using namespace sf;
using namespace std;

class CircleCollider : public Collider
{
public:
	float size = 1;
	CircleCollider() {}
	virtual bool Collision(Collider& _other)
	{
		string name = typeid(_other).name();

		if (name.find("Circle") != string::npos) // this is like string compare in c#
		{
			return Circle(dynamic_cast<CircleCollider&>(_other)); // can't see how you would dynamicly cast depending on type, not possible in C++ it seems
		}
	}
	bool Circle(CircleCollider& _other) // circle x circle, have overload func?!?
	{
		float distance = sqrt(pow((_other.transform->getPosition().x + _other.offsetPos.x) -
			(transform->getPosition().x + offsetPos.x), 2) + pow((_other.transform->getPosition().y + _other.offsetPos.y) - (transform->getPosition().y + offsetPos.y), 2));

		if (distance < abs(_other.size + size))
		{
			return true;
		}
		return false;
	}

	bool operator == (const CircleCollider& Ref) const
	{
		return(this->Member == Ref.GetMember());
	}

	const int GetMember() const
	{
		return(this->Member);
	}

private:
	int Member;
};
