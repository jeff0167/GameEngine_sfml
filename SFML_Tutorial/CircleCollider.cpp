#include "CircleCollider.h"
#include <cmath>
#include <iostream>

using namespace std;
using namespace sf;

bool CircleCollider::Collision(Collider& _other) // here we straight up just have 2 circle colliders, we need to check if they collide
{
	// we have position
	// we literally just need to check for the distance between them

	float distance = sqrt(pow(_other.transform->getPosition().x - transform->getPosition().x, 2) + pow(_other.transform->getPosition().y - transform->getPosition().y, 2)); // might need to make it positive, absolute number

	//cout << "\n";
	//cout << distance;
	//cout << "\n";


	if (distance < abs(dynamic_cast<CircleCollider&>(_other).size + size)) // if distance is less than size then we are colliding !I'M CASTING IT AS CIRCLE COLLIDER FOR NOW, NEED SWITCH STATEMENT
	{
		return true;
	}
	return false;
}