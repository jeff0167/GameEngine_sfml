#include "Physics.h"
#include "Canvas.h"
#include "Mathf.h"
#include "Debug.h"
#include "Pch.h"
#include <atomic>
#include <chrono>

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

void Physics::InitializePhysicsUpdate() 
{
	thread t = std::thread([this] { this->PhysicsUpdate(); });
	t.detach();
}

void Physics::PhysicsUpdate() // physics update is around 0.02 / 50 times pr second, really really really use deltatime for smooth movement, you can't count on this to sleep or be done the apropiate amount every single time
{
	// it might just be that you cant call it at a constant interval
	// you would have to take deltatime into considerration always when moving objects
	// perhaps you could check for a maximum time for which if the calculation takes longer than that we just stop and calculate them next frame
	while (true) // 120 fps is around 0.008333 in ms
	{
		auto step = chrono::system_clock::now() + PhysicsTimeStep; // would like to add the PhysicsTimeStep instead of the manual 0.020s though not sure what data type it is

		PhysicsMovementUpdate();
		ParticleUpdate();            // can't comment this line out, without getting heap error
		PhysicsCollisionUpdate();

		this_thread::sleep_until(step); // it just seems to not call it at the same interval, and it jitters and doesn't feel smooth and responsive

		//_clock.restart();

		//PhysicsMovementUpdate();
		//ParticleUpdate(); 
		//PhysicsCollisionUpdate();

		//_time = _clock.getElapsedTime();
		//double remainingTime = PhysicsTimeStep - _time.asSeconds(); 
		//if (remainingTime >= 0)
		//{		
		//	this_thread::sleep_for(chrono::microseconds((long)remainingTime));
		//}
	}
}

void Physics::AddParticleSystem(ParticleSystem& _particleSystem) 
{
	particleSystems.push_back(&_particleSystem);
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

void Physics::PhysicsMovementUpdate()
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

void Physics::ParticleUpdate() // this will run it's own time and loop and particel system update will get that deltatime, or no the time will be constant and independent of the framerate
{
	for (size_t i = 0; i < particleSystems.size(); i++)
	{
		//Debug::GetInstance()->Log(PhysicsStepTime);
		particleSystems[i]->Update(sf::milliseconds(ParticleTime * 10)); // this will be dependent on the speed of the physicsUpdate, this is technically their movement speed
	}
}

void Physics::PhysicsCollisionUpdate()
{
	//Debug::GetInstance()->Log(colliders.size());
	vector<Collider*> tempColliderList = colliders;

	for (size_t i = 0; i < tempColliderList.size(); i++)
	{
		for (size_t j = 0; j < tempColliderList.size(); j++)
		{
			if (i == j) continue; // but the above line should remove itself also
			//Debug::GetInstance()->Log("Calling cool, dyn");
			if (Mathf::Collision(*colliders[i], *colliders[j]))
			{
				//Debug::GetInstance()->Log("Collision");
			}
		}
		tempColliderList.erase(next(tempColliderList.begin(), i), next(tempColliderList.begin(), i + 1));
	}

	//for (size_t i = 0; i < colliders.size(); i++) // 16,670 vs 40k with 4 colliders  2.4 x difference
	//{
	//	for (size_t j = 0; j < colliders.size(); j++) 
	//	{
	//		if (i == j) continue; // don't check against itself

	//		Debug::GetInstance()->Log("Calling cool, no dyn");
	//		//if (Mathf::Collision(*colliders[i], *colliders[j]))  // ok we collided, now what?!?! we should move the object that moved into it in the oposite direction of it's velocity vector just so it touches the static collider
	//		//{
	//		//	// would want to add that we subsscibe delegates to this func on gameobjects so we can call stuff when it collides with onCollision()

	//		//	// if one is static, meaning it will never move, the other will move in the opposite direction of it's velocity until the point the colliders just touch

	//		//	// if both can move, but only one is moving, the one that stays still will be moved in the opposite direction of the velocity until the point the colliders just touch

	//		//	// if both can move and they both move, the one with the higher velocity will move the other by it's velocity minus the others velocity, 
	//		//	//Debug::GetInstance()->Log("Collision");
	//		//}
	//	}
	//}
}