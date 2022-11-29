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
		float left1, right1, top1, bottom1;
		float left2, right2, top2, bottom2;

		right1 = (first.transform->getPosition().x + first.offsetPos.x) + first.size;
		left1 = (first.transform->getPosition().x + first.offsetPos.x) - first.size;

		right2 = (second.transform->getPosition().x + second.offsetPos.x) + second.size;
		left2 = (second.transform->getPosition().x + second.offsetPos.x) - second.size;

		top1 = (first.transform->getPosition().y + first.offsetPos.y) + first.size;
		bottom1 = (first.transform->getPosition().y + first.offsetPos.y) - first.size;

		top2 = (second.transform->getPosition().y + second.offsetPos.y) + second.size;
		bottom2 = (second.transform->getPosition().y + second.offsetPos.y) - second.size;

		if (left2 > right1) // x=1  x= 2
		{
			//Debug::GetInstance("")->Log("xbox not collided");
			if (left1 < right2) return false;
		}

		if (left1 > right2) // x=1  x= 2
		{
			//Debug::GetInstance("")->Log("xbox not collided");
			if (left2 < right1) return false;
		}

		if (top2 < bottom1) // x=1  x= 2
		{
			//Debug::GetInstance("")->Log("xbox not collided");
			if (top1 > bottom2) return false;
		}

		if (top1 < bottom2) // x=1  x= 2 
		{
			//Debug::GetInstance("")->Log("xbox not collided");
			if (top2 > bottom1) return false;
		}

		//Debug::GetInstance("")->Log("xbox is collided");

		double xi, xj; // we would also want to not move when colliding with static objects

		xi = (first.rigidbody != NULL) ? first.rigidbody->Magnitude() : 0;
		xj = (second.rigidbody != NULL) ? second.rigidbody->Magnitude() : 0;

		if (xi < xj)  // j/right is greater    //we want the  i to move away from j
		{
			if (first.getGlobalBounds().intersects(second.getGlobalBounds())) {

			Debug::GetInstance()->Log("bound intersect");
			}
			else
			{
				Debug::GetInstance()->Log("bound not intersect");
			}

			// technically not acurate way to do it, but works fine for now
			first.transform->move(second.rigidbody->velocity); // i think this happens twice, hence why it's not acurate or maybe the collider is a bit off   
		}

		return true;
	}

	static bool CircleXCircle(CircleCollider& first, CircleCollider& second) // circle x circle
	{
		double distance = sqrt(pow((first.transform->getPosition().x + first.offsetPos.x) -
			(second.transform->getPosition().x + second.offsetPos.x), 2) + pow((first.transform->getPosition().y + first.offsetPos.y) - (second.transform->getPosition().y + second.offsetPos.y), 2));

		if (distance < abs(first.size + second.size))
		{
			return true;
		}
		return false;
	}
	static bool CircleXBox(CircleCollider& first, BoxCollider& second) // circle x circle
	{
		double distance = sqrt(pow((first.transform->getPosition().x + first.offsetPos.x) -
			(second.transform->getPosition().x + second.offsetPos.x), 2) + pow((first.transform->getPosition().y + first.offsetPos.y) - (second.transform->getPosition().y + second.offsetPos.y), 2));

		if (distance < abs(first.size + second.size))
		{
			return true;
		}
		return false;
	}

};