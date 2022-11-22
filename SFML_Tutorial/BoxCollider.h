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
	virtual bool Collision(Collider& _other)
	{
		return false;
	}

	bool operator == (const BoxCollider& Ref) const
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
