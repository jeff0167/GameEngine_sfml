#pragma once
#include <SFML/Graphics.hpp>
#include "Rigidbody.h"
#include "Collider.h"
#include "ParticleSystem.h"

using namespace sf;
using namespace std;

class Physics // needs to be a singleton and have an observer pattern
{
public:
	void Physcis(Physics& other) = delete; // Singletons should not be assignable

	void operator=(const Physics&) = delete;

	// This is the static method that controls the access to the singleton
	// instance. On the first run, it creates a singleton object and places it
	// into the static field. On subsequent runs, it returns the client existing
	// object stored in the static field

	static Physics* GetInstance();

	void InitializePhysicsUpdate();
	//void PhysicsUpdate();

	vector<Rigidbody*> rigidbodies;
	const vector<Rigidbody*>& GetRigidbodies();
	void AddRigidbody(Rigidbody& _rigidbody);
	void RemoveRigidbody(Rigidbody& _rigidbody);
	//void PhysicsMovementUpdate();

	vector<Collider*> colliders; // need to use dynamic programming when checking for collisions, though for a simple test with only a handfull of colliders it will do fine
	const vector<Collider*>& GetColliders();
	void AddCollider(Collider& _collider);
	void RemoveCollider(Collider& _collider);
	//void PhysicsCollisionUpdate();

	vector<ParticleSystem*> particleSystems;
	const vector<ParticleSystem*>& GetParticleSystems();
	void AddParticleSystem(ParticleSystem& _particleSystem);
	void RemoveParticleSystem(ParticleSystem& _particleSystem);

protected:
	static Physics* _physics;
	//const double PhysicsTimeStep2 = 0.02;
	const chrono::duration<double> PhysicsTimeStep = 0.008333s; // 0.02 for 50 times pr sec 0.008333 for 120 times pr sec, equal to defualt fps limit
	double ParticleTime = 2.5;
	Time _time;
	Clock _clock;

	Physics() {}; // constructor

	void PhysicsUpdate();
	void ParticleUpdate();
	void PhysicsMovementUpdate();
	void PhysicsCollisionUpdate();

};
