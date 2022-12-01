#include "Physics.h"
#include "Canvas.h"
#include "Mathf.h"
#include "EmptyComponent.h"
#include "Debug.h"
#include "Pch.h"

using namespace sf;
using namespace std;

Physics* Physics::_physics = nullptr;

Physics* Physics::GetInstance()
{
	if (_physics == nullptr) {
		_physics = new Physics();
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

const vector<Rigidbody*>& Physics::GetRigidbodies()
{
	return rigidbodies;
}

void Physics::PhysicsUpdate()
{
	for (size_t i = 0; i < rigidbodies.size(); i++)
	{
		rigidbodies[i]->gameObject->MoveGameObject(rigidbodies[i]->velocity);
		//rigidbodies[i]->transform->move(rigidbodies[i]->velocity);
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

const vector<Collider*>& Physics::GetColliders()
{
	return colliders;
}

void Physics::PhysicsCollisionUpdate()
{
	//Debug::GetInstance()->Log(colliders.size());
	for (size_t i = 0; i < colliders.size(); i++)
	{
		for (size_t j = 0; j < colliders.size(); j++)  // really need to aply dynamic programming real soon
		{
			if (i == j) continue; // don't check against itself

			if (Mathf::Collision(*colliders[i], *colliders[j]))  // ok we collided, now what?!?! we should move the object that moved into it in the oposite direction of it's velocity vector just so it touches the static collider
			{
				// would want to add that we subsscibe delegates to this func on gameobjects so we can call stuff when it collides with onCollision()


				// if one is static, meaning it will never move, the other will move in the opposite direction of it's velocity until the point the colliders just touch

				// if both can move, but only one is moving, the one that stays still will be moved in the opposite direction of the velocity until the point the colliders just touch

				// if both can move and they both move, the one with the higher velocity will move the other by it's velocity minus the others velocity, 
				//Debug::GetInstance()->Log("Collision");
			}
		}
	}
}