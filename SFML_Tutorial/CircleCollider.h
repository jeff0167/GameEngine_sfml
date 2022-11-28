#pragma once
#include <SFML/Graphics.hpp>
#include "Collider.h"

using namespace sf;
using namespace std;

class CircleCollider : public Collider
{
public:
	float size = 1;
	CircleCollider() {}

	virtual std::size_t getPointCount() const override;


	virtual Vector2f getPoint(std::size_t index) const  override;

	virtual bool Collision(Collider& _other)
	{
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
