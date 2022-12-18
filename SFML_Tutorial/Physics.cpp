#include "Physics.h"
#include "Canvas.h"
#include "Mathf.h"
#include "Debug.h"
#include "Pch.h"
#include <atomic>
#include <chrono>
#include "MyParticle.h"

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

void Physics::PhysicsUpdate() // physics update is around 0.02 / 50 times pr second, really use deltatime for smooth movement, you can't count on this to sleep or be done the apropiate amount every single time
{
	double x = 1 / PhysicsTimeStep.count(); 
	m_DeltaSpeed = 500.0 / x; 
	chrono::time_point<chrono::system_clock, chrono::duration<double, chrono::system_clock::period>> step; // oh boy, a long type, saved as cache for reuse though

	while (true) // 120 fps is around 0.008333 in ms
	{
		step = chrono::system_clock::now() + PhysicsTimeStep; // would like to add the PhysicsTimeStep instead of the manual 0.020s though not sure what data type it is

		UpdateTime();
		ParticleUpdate();    // the jittering seems to be worse without calling this!?!?      
		PhysicsMovementUpdate(); // what if the draw function gets called right in between this func and the collision func? it would mean it would draw the movement frame and in the collision the position could have changed
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

void Physics::AddParticleSystem(ParticleSystemUpdate& _particleSystem) 
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
	double x = m_PhysicsDeltaTime / 1 / PhysicsTimeStep.count(); // really not very clean code, the timeStep is acurate to a certain point, if keept within a reasonable range the difference is not very significant, 0.01 or 0.02 is not noticable in difference
	m_DeltaSpeed = (500.0 / x) * 0.01f;

	for (size_t i = 0; i < rigidbodies.size(); i++)
	{
		// the deltaSpeed changes, sure, but the m_PhysicsDeltaTime should multiplied with the deltaSpeed, give the exact same value
		// Now deltaSpeed is constant and physicsDelta is not, meaning that deltaSpeed shouldn't be constant, it should be calculated for each physics update

		rigidbodies[i]->transform->move(rigidbodies[i]->velocity * (float)m_DeltaSpeed * m_PhysicsDeltaTime * 100.0f); // do we lose precision?
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

void Physics::ParticleUpdate()
{
	for (const auto& particleSystem : particleSystems) // foreach
	{
		// milliseconds(ParticleTime * deltaSpeed) was put in update before, the movement should technically be dependent on the physics time step
		// for now it is realying on the delta time between frames
		particleSystem->Update(); // this will be dependent on the speed of the physicsUpdate, this is technically their movement speed
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
			if (i == j) continue; 
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