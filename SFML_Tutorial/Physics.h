#pragma once
#include <SFML/Graphics.hpp>
#include "Rigidbody.h"
#include "Collider.h"
#include "ParticleSystem.h"

using namespace sf;
using namespace std;

class Physics 
{
public:
	void Physcis(Physics& other) = delete; 

	void operator=(const Physics&) = delete;

	static Physics* GetInstance();

	void InitializePhysicsUpdate();

	vector<Rigidbody*> rigidbodies;
	const vector<Rigidbody*>& GetRigidbodies();
	void AddRigidbody(Rigidbody& _rigidbody);
	void RemoveRigidbody(Rigidbody& _rigidbody);

	vector<Collider*> colliders; // need to use dynamic programming when checking for collisions, though for a simple test with only a handfull of different colliders it will do fine
	const vector<Collider*>& GetColliders();
	void AddCollider(Collider& _collider);
	void RemoveCollider(Collider& _collider);

	vector<ParticleSystem*> particleSystems;
	const vector<ParticleSystem*>& GetParticleSystems();
	void AddParticleSystem(ParticleSystem& _particleSystem);
	void RemoveParticleSystem(ParticleSystem& _particleSystem);

	double deltaSpeed;
protected:
	static Physics* _physics;
	//const double PhysicsTimeStep2 = 0.02;
	const chrono::duration<double> PhysicsTimeStep = 0.01s; // 0.02 for 50 times pr sec 0.008333 for 120 times pr sec, equal to defualt fps limit
	double ParticleTime = 2.5;
	Time _time;
	Clock _clock;

	Physics() {}; 

	void PhysicsUpdate();
	void ParticleUpdate();
	void PhysicsMovementUpdate();
	void PhysicsCollisionUpdate();

};
