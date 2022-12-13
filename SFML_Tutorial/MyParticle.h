#pragma once
#include <SFML/Graphics.hpp>
#include <future>
#include <mutex>
#include "Canvas.h"
#include "Debug.h"




#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <future>
#include <string>
#include <mutex>

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

	static void InitializeParticle(Particle* particle, unsigned int radius, float _speed, Time lifeTime) // don't need to lock with mutex here
	{
		float size = (rand() % 3) * radius;
		particle->dot.setRadius(size);
		float angle = (rand() % 360) * 3.14f / 180.f;
		float speed = (rand() % 50) * 0.01 * _speed + 0.05; 
		particle->SetVelocity(cos(angle) * speed, sin(angle) * speed);

		particle->maxLifespan = milliseconds((rand() % 200) + 100) * lifeTime.asSeconds();

		//lock_guard<mutex> lock(m_Mutex);
	}

	vector<future<void>> m_Futures;
	MyParticleSystem(Transformable* targetTransform, unsigned int particleCount, unsigned int radius, Texture& texture, float speed, Time lifeTime, Color color) :
		m_TargetTransform(targetTransform),
		m_particles(particleCount),
		m_Texture(&texture),
		m_Speed(speed),
		m_Radius(radius),
		m_LifeTime(lifeTime)

	{
		//for (size_t i = 0; i < m_particles.size(); ++i) // dude this is a huge function, duuuuuude async!!
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


		for (size_t i = 0; i < m_particles.size(); ++i)
		{
			Canvas::GetInstance()->AddDrawable(m_particles[i].dot);
			m_particles[i].dot.setTexture(&texture);
			m_particles[i].SetParticleSystem(*this);
		}

		for (size_t i = 0; i < m_particles.size(); ++i)  // now to test if it became faster
		{
			m_Futures.push_back(async(launch::async, InitializeParticle, &m_particles[i], radius, speed, lifeTime));
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
		for (size_t i = 0; i < m_particles.size(); ++i) // dude, I could literally use async for this loop!!!!
		{
			m_particles[i].Update();
		}
	}

protected:
	vector<Particle> m_particles;
	Transformable* m_TargetTransform;
	const Texture* m_Texture;
	float m_Speed, m_Radius;
	Time m_LifeTime;

};