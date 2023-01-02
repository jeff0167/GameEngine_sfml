#pragma once
#include <SFML/Graphics.hpp>
#include <future>
#include <mutex>
#include "Canvas.h"
#include "Monobehaviour.h"
#include "Debug.h"
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
	void SetPosition(Vector2f pos);
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

	static void InitializeParticle(Particle* particle, Texture* texture, Vector2f pos, float radius, float _speed, Time lifeTime) // don't need to lock with mutex here
	{
		particle->dot.setTexture(texture);
		float size = (rand() % 3) * radius;
		particle->dot.setRadius(size);
		float angle = (rand() % 360) * 3.14f / 180.f;
		float speed = (rand() % 50) * 0.01 * _speed + 0.05;
		particle->SetVelocity(cos(angle) * speed, sin(angle) * speed);
		particle->SetMaxLifeTime(milliseconds((rand() % 100) * 2.0f + lifeTime.asMilliseconds()));
		particle->lifespan = particle->maxLifespan;
		particle->SetPosition(pos);
		//lock_guard<mutex> lock(m_Mutex); // Get lower performance if used and isn't needed to make it work it seems
	}

	vector<future<void>> m_Futures;
	MyParticleSystem(Transformable* targetTransform, unsigned int particleCount, float radius, Texture& texture, float _speed, Time lifeTime, Color color) :
		m_TargetTransform(targetTransform),
		m_particles(particleCount),
		m_Texture(&texture),
		m_Speed(_speed),
		m_Radius(radius),
		m_LifeTime(lifeTime)
	{
		//auto d = Mono->Timer();

		//for (auto& particle : m_particles)  // I get 130-150 withoput async and 15-30 with async with 2k particles, pog
		//{
		//	Canvas::GetInstance()->AddDrawable(particle.dot);
		//	particle.SetParticleSystem(*this);
		//	particle.dot.setTexture(&texture);
		//	float size = (rand() % 3) * radius;
		//	particle.dot.setRadius(size);
		//	float angle = (rand() % 360) * 3.14f / 180.f;
		//	float speed = (rand() % 50) * 0.01 * _speed + 0.05;
		//	particle.SetVelocity(cos(angle) * speed, sin(angle) * speed);
		//	particle.SetMaxLifeTime(milliseconds((rand() % 100) * 2.0f + lifeTime.asMilliseconds()));
		//  particle.lifespan = particle->maxLifespan;
		//  particle.SetPosition(m_TargetTransform->getPosition());
		//}

		for (auto& particle : m_particles)
		{
			Renderer->AddDrawable(particle.dot);
			particle.SetParticleSystem(*this);
			m_Futures.push_back(async(launch::async, InitializeParticle, &particle, &texture, m_TargetTransform->getPosition(), radius, _speed, lifeTime));
		}
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
		for (auto& particle : m_particles) 
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