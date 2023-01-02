#include <SFML/Graphics.hpp>
#include "Physics.h"
#include "Pch.h"
#include "Mathf.h"
#include "Debug.h"
#include "BoxCollider.h"
#include "CircleCollider.h"
#include <atomic>
#include <chrono>

using namespace sf;
using namespace std;

void AddForce(Rigidbody* rb, Vector2f force);
float FixedUpdateMovement();
bool Collision(Collider& first, Collider& second);
bool BoxXBox(BoxCollider& first, BoxCollider& second);
bool CircleXCircle(CircleCollider& first, CircleCollider& second);
bool CircleXBox(CircleCollider& first, BoxCollider& second);

Physics* m_Physics = Science;

Physics* Physics::_physics = nullptr;

Physics* Physics::GetInstance()
{
	if (_physics == nullptr)
	{
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
	m_DeltaSpeed = (500.0 / x) * 0.0100;

	float finalDeltaSpeed = FixedUpdateMovement(); 

	for (const auto& rigidbody : rigidbodies)
	{
		// the deltaSpeed changes, sure, but the m_PhysicsDeltaTime should multiplied with the deltaSpeed, give the exact same value
		// Now deltaSpeed is constant and physicsDelta is not, meaning that deltaSpeed shouldn't be constant, it should be calculated for each physics update
		rigidbody->transform->move(rigidbody->velocity * finalDeltaSpeed);

		// then comes the question when do you apply the gravity within the physics loop? no movement should be done after collision tough
		if (rigidbody->useGravity) AddForce(rigidbody, gravity * m_PhysicsDeltaTime); // Hmm should perhaps have it so you have a list of all objects that use gravity, so you dont do the check
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
	//DebugLog(colliders.size());
	vector<Collider*> tempColliderList = colliders;

	for (size_t i = 0; i < tempColliderList.size(); i++)
	{
		for (size_t j = 0; j < tempColliderList.size(); j++)
		{
			if (i == j) continue;
			//DebugLog("Calling cool, dynamic");
			if (Collision(*colliders[i], *colliders[j]))
			{
				//DebugLog("Collision");
			}
		}
		tempColliderList.erase(next(tempColliderList.begin(), i), next(tempColliderList.begin(), i + 1));
	}
}

void AddForce(Rigidbody* rb, Vector2f force)
{
	rb->velocity += force;
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
		return BoxXBox(static_cast<BoxCollider&>(first), static_cast<BoxCollider&>(second)); // dynamic cast is slower because it has to check the type and will return null if false
	}
	if (d.x == 2 && d.y == 2) // circle x circle
	{
		return CircleXCircle(static_cast<CircleCollider&>(first), static_cast<CircleCollider&>(second));
	}
	if ((d.x == 2 && d.y == 1)) // circle x box
	{
		return CircleXBox(static_cast<CircleCollider&>(first), static_cast<BoxCollider&>(second));
	}
	if ((d.x == 1 && d.y == 2))
	{
		return CircleXBox(static_cast<CircleCollider&>(second), static_cast<BoxCollider&>(first));
	}
	return false;
}

// cleans up the code a bit, though not sure if it would impact the performance
static float FixedUpdateMovement()
{
	return (float)m_Physics->m_DeltaSpeed * m_Physics->m_PhysicsDeltaTime * 100.0f;
}

static bool BoxXBox(BoxCollider& first, BoxCollider& second) // box x box
{
	double xi, xj; // we would also want to not move when colliding with static objects
	xi = (first.rigidbody != NULL) ? first.rigidbody->Magnitude() : 0;
	xj = (second.rigidbody != NULL) ? second.rigidbody->Magnitude() : 0;

	if (first.rect->getGlobalBounds().intersects(second.rect->getGlobalBounds())) // we are not able to use offset, should the gameObject's transform be an offset of the shape instead?!?
	{
		//if (xi == 0 || xj == 0) return true;
		if (xi > xj)
		{
			// this might need some updates values, or other values, seems to work alright atm
			second.rigidbody->transform->move(first.rigidbody->velocity * FixedUpdateMovement());
		}
		else first.rigidbody->transform->move(second.rigidbody->velocity * FixedUpdateMovement());
		//Debug::GetInstance()->Log("bound intersect");
		return true;
	}
	return false;
}
static bool CircleXCircle(CircleCollider& first, CircleCollider& second) // circle x circle
{
	double xi, xj;
	xi = (first.rigidbody != NULL) ? first.rigidbody->Magnitude() : 0;
	xj = (second.rigidbody != NULL) ? second.rigidbody->Magnitude() : 0;

	double distance = sqrt(pow((first.rect->getPosition().x + first.offsetPos.x) -
		(second.rect->getPosition().x + second.offsetPos.x), 2) + pow((first.rect->getPosition().y + first.offsetPos.y) - (second.rect->getPosition().y + second.offsetPos.y), 2));

	if (distance < abs(first.rect->getRadius() + second.rect->getRadius()))
	{
		//if (xi == 0 || xj == 0) return true;
		if (xi > xj)
		{
			second.rigidbody->transform->move(first.rigidbody->velocity * FixedUpdateMovement());
		}
		else first.rigidbody->transform->move(second.rigidbody->velocity * FixedUpdateMovement());
		//Debug::GetInstance()->Log("Circle collision");
		return true;
	}
	return false;
}
static bool CircleXBox(CircleCollider& first, BoxCollider& second) // circle x box
{
	double xi, xj; // we would also want to not move when colliding with static objects
	xi = (first.rigidbody != NULL) ? first.rigidbody->Magnitude() : 0;
	xj = (second.rigidbody != NULL) ? second.rigidbody->Magnitude() : 0;

	if (first.rect->getGlobalBounds().intersects(second.rect->getGlobalBounds()))
	{
		//if (xi == 0 || xj == 0) return true;

		if (xi > xj)
		{
			second.rigidbody->transform->move(first.rigidbody->velocity * FixedUpdateMovement());
		}
		else first.rigidbody->transform->move(second.rigidbody->velocity * FixedUpdateMovement());
		//Debug::GetInstance()->Log("bound intersect");
		return true;
	}
	//Debug::GetInstance()->Log("bound not intersect");
	return false;
}