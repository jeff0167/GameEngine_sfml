#pragma once
#include <SFML/Graphics.hpp>
#include "Canvas.h"
#include "Debug.h"

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
	int lifespan; // this should be in seconds
	int Maxlifespan; // this should be in seconds ///TODO
protected:
	Vector2f velocity;
	Vector2f acceleration;
	Vector2f position, startPos;
	MyParticleSystem* m_particleSystem;

};

class MyParticleSystem : public Component
{
public:
	MyParticleSystem() {};
	MyParticleSystem(Transformable* targetTransform, unsigned int particleCount, unsigned int radius, Texture& texture, float _speed, Color color) :
		m_TargetTransform(targetTransform),
		m_particles(particleCount)
	{
		for (size_t i = 0; i < m_particles.size(); ++i) // dude this is a huge function, duuuuuude async!!
		{
			m_particles[i].dot.setTexture(&texture);
			float size = (rand() % 3) * radius;
			m_particles[i].dot.setRadius(size);
			m_particles[i].SetParticleSystem(*this);
			float angle = (rand() % 360) * 3.14f / 180.f;
			float speed = (rand() % 50) * 0.01 * _speed + 0.05; // takes a long time for it to generate these numbers
			m_particles[i].SetVelocity(cos(angle) * speed, sin(angle) * speed);

			m_particles[i].Maxlifespan = (rand() % 200) + 100;

			Canvas::GetInstance()->AddDrawable(m_particles[i].dot);
		}
	}

	void SetEmitterTransform(Transformable& transform)
	{
		m_TargetTransform = &transform;
	}
	Vector2f GetTargetTransform()
	{
		if (m_TargetTransform != nullptr) return m_TargetTransform->getPosition();
		return Vector2f(0, 0);
	}

	void Update()
	{
		for (size_t i = 0; i < m_particles.size(); ++i) // dude, I could literally use async for this loop!!!!
		{
			m_particles[i].Update();
		}
	}

protected:
	vector<Particle> m_particles;
	Transformable* m_TargetTransform;

};