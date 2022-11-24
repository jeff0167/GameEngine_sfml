#include "Physics.h"
#include "Canvas.h"
#include "Mathf.h"
#include "EmptyComponent.h"
#include "Debug.h"
#include "Pch.h"

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

const vector<Rigidbody*>& Physics::GetRigidbodies()
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

const vector<Collider*>& Physics::GetColliders()
{
	return colliders;
}

void Physics::PhysicsCollisionUpdate()
{
	for (size_t i = 0; i < colliders.size(); i++)
	{
		for (size_t j = 0; j < colliders.size(); j++)  // really need to aply dynamic programming real soon
		{
			if (i == j) continue; // don't check against itself

			if (Mathf::Collision(*colliders[i], *colliders[j]))  // ok we collided, now what?!?! we should move the object that moved into it in the oposite direction of it's velocity vector just so it touches the static collider
			{
				// if one is static, meaning it will never move, the other will move in the opposite direction of it's velocity until the point the colliders just touch

				// if both can move, but only one is moving, the one that stays still will be moved in the opposite direction of the velocity until the point the colliders just touch

				double xi, xj;

				xi = (colliders[i]->rigidbody != NULL) ? colliders[i]->rigidbody->Magnitude() : 0; 
				xj = (colliders[j]->rigidbody != NULL) ? colliders[j]->rigidbody->Magnitude() : 0; 

				if (xi < xj)  // j/right is greater    //we want the  i to move away from j
				{
					auto d = colliders[j]->rigidbody->velocity; // if press x and y at the same time then it disapears!!

					// dude i'm just moving it at the same paze as the other collider, that's why they don't ever touch
					colliders[i]->transform->move(Mathf::Normalize(d)); // i think this happens twice, hence why it's not acurate or maybe the collider is a bit off   
				}

				// if both can move and they both move, the one with the higher velocity will move the other by it's velocity minus the others velocity, 

				//Debug::GetInstance("")->Log("Collide");
			}
		}
	}
}