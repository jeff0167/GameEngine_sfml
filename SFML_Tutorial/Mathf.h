#pragma once
#include <SFML/Graphics.hpp>
#include "BoxCollider.h"
#include "CircleCollider.h"
#include "Collider.h"
#include "Debug.h"

using namespace sf;
using namespace std;

class Mathf
{
public:
	static Vector2f Zero()
	{
		return Vector2f(0, 0);
	}

	static double Magnitude(Vector2f& velocity)
	{
		return sqrt(pow(velocity.x, 2) + pow(velocity.y, 2));
	}

	static Vector2f Normalize(Vector2f& source)
	{
		double length = Magnitude(source);

		if (source.x == 0 && source.y == 0) return source;

		source.x = source.x / length;
		source.y = source.y / length;
		return source;
	}

	static Vector2f DirectionVector(Vector2f start, Vector2f end)
	{
		return Vector2f(start.x - end.x, start.y - end.y);
	}

	static bool Collision(Collider& first, Collider& second)
	{
		string firstName = typeid(first).name();
		string secondName = typeid(second).name();

		Vector2f d(0, 0);

		if (firstName.find("Box") != string::npos)
		{
			d.x = 1;
		}

		if (secondName.find("Box") != string::npos)
		{
			d.y = 1;
		}

		if (firstName.find("Circle") != string::npos)
		{
			d.x = 2;
		}

		if (secondName.find("Circle") != string::npos)
		{
			d.y = 2;
		}

		if (d.x == 1 && d.y == 1) // box x box
		{
			return BoxXBox(dynamic_cast<BoxCollider&>(first), dynamic_cast<BoxCollider&>(second));
		}
		if (d.x == 2 && d.y == 2) // circle x circle
		{
			return CircleXCircle(dynamic_cast<CircleCollider&>(first), dynamic_cast<CircleCollider&>(second));
		}

		if ((d.x == 2 && d.y == 1)) // circle x box
		{
			return CircleXBox(dynamic_cast<CircleCollider&>(first), dynamic_cast<BoxCollider&>(second));
		}
		if ((d.x == 1 && d.y == 2))
		{
			return CircleXBox(dynamic_cast<CircleCollider&>(second), dynamic_cast<BoxCollider&>(first));
		}
		return false;
	}

private:
	static bool BoxXBox(BoxCollider& first, BoxCollider& second) // box x box
	{
		double xi, xj; // we would also want to not move when colliding with static objects
		xi = (first.rigidbody != NULL) ? first.rigidbody->Magnitude() : 0;
		xj = (second.rigidbody != NULL) ? second.rigidbody->Magnitude() : 0;

		if (first.rect->getGlobalBounds().intersects(second.rect->getGlobalBounds())) 
		{
			//if (xi == 0 || xj == 0) return true;
			if (xi > xj)
			{
				second.rigidbody->transform->move(first.rigidbody->velocity);
			}
			else first.rigidbody->transform->move(second.rigidbody->velocity);
			//Debug::GetInstance()->Log("bound intersect");
			return true;
		}
		return false;
	}

	static bool CircleXCircle(CircleCollider& first, CircleCollider& second) // circle x circle
	{	
		double xi, xj;
		xi = (first.rigidbody != NULL) ? first.rigidbody->Magnitude() : 0;
		xj = (second.rigidbody != NULL) ? second.rigidbody->Magnitude() : 0;

		double distance = sqrt(pow((first.rect->getPosition().x + first.offsetPos.x) -
			(second.rect->getPosition().x + second.offsetPos.x), 2) + pow((first.rect->getPosition().y + first.offsetPos.y) - (second.rect->getPosition().y + second.offsetPos.y), 2));

		if (distance < abs(first.rect->getRadius() + second.rect->getRadius()))
		{
			//if (xi == 0 || xj == 0) return true;
			if (xi > xj)
			{
				second.rigidbody->transform->move(first.rigidbody->velocity);
			}
			else first.rigidbody->transform->move(second.rigidbody->velocity);
			//Debug::GetInstance()->Log("Circle collision");
			return true;
		}
		return false;
	}
	static bool CircleXBox(CircleCollider& first, BoxCollider& second) // circle x box
	{
		double xi, xj; // we would also want to not move when colliding with static objects
		xi = (first.rigidbody != NULL) ? first.rigidbody->Magnitude() : 0;
		xj = (second.rigidbody != NULL) ? second.rigidbody->Magnitude() : 0;

		if (first.rect->getGlobalBounds().intersects(second.rect->getGlobalBounds())) {
			//if (xi == 0 || xj == 0) return true;

			if (xi > xj)
			{
				second.rigidbody->transform->move(first.rigidbody->velocity);
			}
			else first.rigidbody->transform->move(second.rigidbody->velocity);
			//Debug::GetInstance()->Log("bound intersect");
			return true;
		}
		//Debug::GetInstance()->Log("bound not intersect");
		return false;
	}

};