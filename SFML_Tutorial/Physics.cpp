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

vector<Rigidbody*> Physics::GetRigidbodies()
{
	return rigidbodies;
}

void Physics::PhysicsUpdate()
{
	for (size_t i = 0; i < rigidbodies.size(); i++)
	{
		rigidbodies[i]->transform->move(rigidbodies[i]->velocity); // original
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
		for (size_t j = 0; j < colliders.size(); j++)  // really need to aply dynamic programming real soon
		{
			if (i == j) continue; // don't check against itself

			if (Mathf::Collision(*colliders[i], *colliders[j]))  // ok we collided, now what?!?! we should move the object that moved into it in the oposite direction of it's velocity vector just so it touches the static collider
			{
				// if one is static, meaning it will never move, the other will move in the opposite direction of it's velocity until the point the colliders just touch

				// if both can move, but only one is moving, the one that stays still will be moved in the opposite direction of the velocity until the point the colliders just touch

				double xi, xj;

				auto ci = colliders[i]->gameObject->components;
				auto cj = colliders[j]->gameObject->GetComponents();

				for (size_t l = 0; l < ci.size(); l++)
				{
					Debug::GetInstance("")->Log(typeid(ci[l]).name());
				}

				xi = colliders[i]->rb->Magnitude();
				xj = colliders[j]->rb->Magnitude();


				//xi = (colliders[i]->gameObject->GetComponent(ci, Rigidbody()) != nullptr) ? dynamic_cast<Rigidbody&>(*colliders[i]->gameObject->GetComponent(ci, Rigidbody())).Magnitude() : 0; // the compiler can't compile these lines
				//xj = (colliders[j]->gameObject->GetComponent(cj, Rigidbody()) != nullptr) ? dynamic_cast<Rigidbody&>(*colliders[j]->gameObject->GetComponent(cj, Rigidbody())).Magnitude() : 0; // rigidbody something?!!?

				//if (colliders[i]->gameObject->GetComponent(ci, Rigidbody()) == NULL || colliders[j]->gameObject->GetComponent(cj, Rigidbody()) == NULL) return;

				//Debug::GetInstance("")->Log(dynamic_cast<Rigidbody&>(*colliders[i]->gameObject->GetComponent(ci, Rigidbody())).Magnitude());
				//Debug::GetInstance("")->Log(dynamic_cast<Rigidbody&>(*colliders[j]->gameObject->GetComponent(cj, Rigidbody())).Magnitude());

				if (xi < xj)  // j/right is greater    //we want the  i to move away from j
				{
					Debug::GetInstance("")->Log("Let's move something");
					//auto d = dynamic_cast<Rigidbody&>(*colliders[j]->gameObject->GetComponent(cj, Rigidbody())).velocity;

					//Vector2f v = Mathf::Normalize(d); // the * operator doesn't work even tough in the documentation it does have the template for it

					//colliders[i]->transform->move(Vector2f(v.x * 25, v.y * 25));
				}

				// if both can move and they both move, the one with the higher velocity will move the other by it's velocity minus the others velocity, 

				//Debug::GetInstance("")->Log("Collide");
			}
		}
	}
}