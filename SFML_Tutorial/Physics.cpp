#include "Physics.h"
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