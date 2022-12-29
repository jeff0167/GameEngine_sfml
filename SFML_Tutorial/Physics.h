#pragma once
#include <SFML/Graphics.hpp>
#include "Rigidbody.h"
#include "Collider.h"
#include "ParticleSystem.h"

#define Science Physics::GetInstance()

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

	vector<ParticleSystemUpdate*> particleSystems;
	const vector<ParticleSystemUpdate*>& GetParticleSystems();
	void AddParticleSystem(ParticleSystemUpdate& _particleSystem);
	void RemoveParticleSystem(ParticleSystemUpdate& _particleSystem);

	double m_DeltaSpeed;
	float m_PhysicsDeltaTime;

	float GetDeltaTimeMili() 
	{
		return m_Time.asMilliseconds();
	}

protected:
	static Physics* _physics;
	const chrono::duration<double> PhysicsTimeStep = 0.02s; // 0.02 for 50 times pr sec 0.008333 for 120 times pr sec, equal to defualt fps limit     //TODO this is now affecting movespeed
	Time m_Time;
	Clock m_Clock;
	Vector2f gravity = Vector2f(0, 9.81);

	void UpdateTime()
	{
		m_Time = m_Clock.getElapsedTime();
		m_Clock.restart();
		m_PhysicsDeltaTime = m_Time.asSeconds();
	}

	Physics() {}; 

	void PhysicsUpdate();
	void ParticleUpdate();
	void PhysicsMovementUpdate();
	void PhysicsCollisionUpdate();

};
