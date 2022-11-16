#include "Physics.h"
#include "Canvas.h"
#include <iostream>

using namespace sf;
using namespace std;

Physics* Physics::_physics = nullptr;

Physics* Physics::GetInstance(const string& value)
{
	if (_physics == nullptr) {
		_physics = new Physics(value);
	}
	return _physics;
}

void Physics::AddRigidbody(Rigidbody& _rigidbody)
{
	rigidbodies.push_back(&_rigidbody);
}

void Physics::RemoveRigidbody(Rigidbody& _rigidbody)
{
	for (size_t i = 0; i < rigidbodies.size(); i++)
	{
		if (rigidbodies[i] == &_rigidbody)
		{
			rigidbodies.erase(next(rigidbodies.begin(), i), next(rigidbodies.begin(), i + 1));
		}
	}
}

vector<Rigidbody*> Physics::GetRigidbodies()
{
	return rigidbodies;
}

void Physics::PhysicsUpdate()
{
	for (size_t i = 0; i < rigidbodies.size(); i++)
	{
		rigidbodies[i]->transform->move(rigidbodies[i]->velocity);
	}
}

void Physics::AddCollider(Collider& _collider)
{
	colliders.push_back(&_collider);
}

void Physics::RemoveCollider(Collider& _collider)
{
	for (size_t i = 0; i < colliders.size(); i++)
	{
		if (colliders[i] == &_collider)
		{
			colliders.erase(next(colliders.begin(), i), next(colliders.begin(), i + 1));
		}
	}
}

vector<Collider*> Physics::GetColliders()
{
	return colliders;
}

void Physics::PhysicsCollisionUpdate()  
{
	//cout << colliders.size();
	for (size_t i = 0; i < colliders.size(); i++)
	{
		for (size_t j = 0; j < colliders.size(); j++) // currently you can check collision against yourself
		{
			if (i == j) continue;
			//cout << "\n";
			//cout << colliders[i]->transform->getPosition().x;
			//cout << "\n";
			//cout << colliders[i]->transform->getPosition().y;
			//cout << "\n";
			//cout << colliders[j]->transform->getPosition().x;
			//cout << "\n";
			//cout << colliders[j]->transform->getPosition().y;
			//cout << "\n";

			if (colliders[i]->Collision(*colliders[j]))  // ok we collided, now what?!?! we should move the objekt that moved into it in the oposite direction of it's velocity vector just so it touches the static collider
			{
				cout << "Something collided, pog"; // how would you do it if 2 colliders move into each other!?!?
			}
		}
	}
}