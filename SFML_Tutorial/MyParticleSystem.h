#pragma once
#include <SFML/Graphics.hpp>
#include <future>
#include <mutex>
#include "Canvas.h"
#include "Debug.h"
#include "Monobehaviour.h"
#include <vector>
#include <algorithm>
#include <numeric>

using namespace sf;
using namespace std;

class MyParticleSystem;

class Particle
{
public:
	CircleShape dot;

	Particle() {};
	Particle(float radius, Texture& texture);
	~Particle();

	void SetVelocity(float x, float y);
	void SetPosition(float x, float y);
	void SetMaxLifeTime(Time time);
	Vector2f GetTargetPos();
	void Update();
	void ApplyForce(Vector2f);
	Vector2f GetVelocity();
	void SetParticleSystem(MyParticleSystem& ps);
	bool IsDead();
	Color SetColor(int, float, float, float);
	Time lifespan, maxLifespan;

protected:
	Vector2f velocity;
	Vector2f acceleration;
	Vector2f position, startPos;
	MyParticleSystem* m_particleSystem;

};

class MyParticleSystem : public Component, public ParticleSystemUpdate
{
public:
	MyParticleSystem() {};

	//static mutex m_Mutex;

	static void InitializeParticle(Particle* particle, Texture* texture, float radius, float _speed, Time lifeTime) // don't need to lock with mutex here
	{
		particle->dot.setTexture(texture);
		float size = (rand() % 3) * radius;
		particle->dot.setRadius(size);
		float angle = (rand() % 360) * 3.14f / 180.f;
		float speed = (rand() % 50) * 0.01 * _speed + 0.05;
		particle->SetVelocity(cos(angle) * speed, sin(angle) * speed);
		particle->SetMaxLifeTime(milliseconds((rand() % 100) * 2.0f + lifeTime.asMilliseconds()));

		//lock_guard<mutex> lock(m_Mutex);
	}

	vector<future<void>> m_Futures;
	MyParticleSystem(Transformable* targetTransform, unsigned int particleCount, float radius, Texture& texture, float speed, Time lifeTime, Color color) :
		m_TargetTransform(targetTransform),
		m_particles(particleCount),
		m_Texture(&texture),
		m_Speed(speed),
		m_Radius(radius),
		m_LifeTime(lifeTime)

	{
		//Time debugTime;
		//Clock debugClock;
		//float debugDeltaTime;

		//debugClock.restart();

		//for (size_t i = 0; i < m_particles.size(); ++i) // 630 ms - 640 with 10k particles
		//{
		//	m_particles[i].dot.setTexture(m_Texture);
		//	float size = (rand() % 3) * m_Radius;
		//	m_particles[i].dot.setRadius(size);
		//	m_particles[i].SetParticleSystem(*this);
		//	float angle = (rand() % 360) * 3.14f / 180.f;
		//	float speed = (rand() % 50) * 0.01 * m_Speed + 0.05; // takes a long time for it to generate these numbers
		//	m_particles[i].SetVelocity(cos(angle) * speed, sin(angle) * speed);

		//	m_particles[i].maxLifespan = milliseconds((rand() % 200) + 100) * m_LifeTime.asSeconds();

		//	Canvas::GetInstance()->AddDrawable(m_particles[i].dot);
		//}

		//debugTime = debugClock.getElapsedTime();
		//debugDeltaTime = debugTime.asMilliseconds();

		//Debug::GetInstance()->Log(debugDeltaTime);

		for (auto& particle : m_particles)
		{
			Renderer->AddDrawable(particle.dot);
			particle.SetParticleSystem(*this);
		}

		for (auto& particle : m_particles)
		{
			m_Futures.push_back(async(launch::async, InitializeParticle, &particle, &texture, radius, speed, lifeTime));
		}

		/*	debugTime = debugClock.getElapsedTime();
			debugDeltaTime = debugTime.asMilliseconds();
			Debug::GetInstance()->Log(debugDeltaTime);*/
	}

	void SetEmitterTransform(Transformable& transform) override
	{
		m_TargetTransform = &transform;
	}
	Vector2f GetTargetTransform()
	{
		if (m_TargetTransform != nullptr) return m_TargetTransform->getPosition();
		return Vector2f(0, 0);
	}

	void Update() override
	{
		for (auto& particle : m_particles) // dude, I could literally use async for this loop, did not work
		{
			particle.Update();
		}
	}

protected:
	vector<Particle> m_particles;
	Transformable* m_TargetTransform;
	const Texture* m_Texture;
	float m_Speed = 0, m_Radius = 0;
	Time m_LifeTime;

};