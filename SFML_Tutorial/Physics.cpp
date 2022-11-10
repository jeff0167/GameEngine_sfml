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

void Physics::AddRigidbody(Rigidbody& _rigidbody) // I need a monobehaviour, dude it makes sense, dont judge the name, to instantiate gameobjects with and other things
{
	rigidbodies.push_back(&_rigidbody);
}

void Physics::RemoveRigidbody(Rigidbody& _rigidbody)
{
	for (size_t i = 0; i < rigidbodies.size(); i++)
	{
		if (rigidbodies[i] == &_rigidbody) // should we make our own texture class so we can check the class type ref, could we check another way?
		{
			rigidbodies.erase(next(rigidbodies.begin(), i), next(rigidbodies.begin(), i + 1)); // really convoluted to erase things from vector, + operator might also not be aloved
		}
	}
}

vector<Rigidbody*> Physics::GetRigidbodies()
{
	return rigidbodies;
}

void Physics::PhysicsUpdate()
{
	for (size_t i = 0; i < rigidbodies.size(); i++) // i need to update it's position by adding the velocity, which means a rigidbody should have a vector2?
	{
		rigidbodies[i]->transform->setPosition(rigidbodies[i]->transform->getPosition() + rigidbodies[i]->velocity); // does not have simple vector * float operator
	}
}