#pragma once
#include <SFML/Graphics.hpp>
#include "Canvas.h"

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

	void setVelocity(float x, float y);
	void setPosition(float x, float y);
	Vector2f GetTargetPos();
	void update();
	void applyForce(Vector2f);
	Vector2f getVelocity();
	void SetParticleSystem(MyParticleSystem& ps);
	bool isDead();
	Color setColor(int, float, float);
	int lifespan;
protected:
	Vector2f velocity;
	Vector2f acceleration;
	Vector2f position, startPos;
	MyParticleSystem* m_particleSystem;

};

class MyParticleSystem
{
public:
	MyParticleSystem() {};
	MyParticleSystem(Transformable* targetTransform, unsigned int particleCount, unsigned int radius, Texture& texture, float _speed, Color color) :
		m_TargetTransform(targetTransform),
		m_particles(particleCount)
	{
		for (size_t i = 0; i < m_particles.size(); ++i)
		{
			m_particles[i].dot.setTexture(&texture);
			m_particles[i].dot.setRadius(radius);
			m_particles[i].SetParticleSystem(*this);
			float angle = (rand() % 360) * 3.14f / 180.f;
			float speed = (float)(rand() % (int)_speed);
			m_particles[i].setVelocity(cos(angle) * speed, sin(angle) * speed);

			m_particles[i].lifespan = (rand() % 200) + 100;

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
		for (size_t i = 0; i < m_particles.size(); ++i)
		{
			m_particles[i].update();
		}
	}

protected:
	vector<Particle> m_particles;
	Transformable* m_TargetTransform;

};