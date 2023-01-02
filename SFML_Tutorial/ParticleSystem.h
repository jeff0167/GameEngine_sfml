#pragma once
#include <SFML/Graphics.hpp>
#include "Canvas.h"
#include "Monobehaviour.h"

using namespace sf;
using namespace std;

class ParticleSystem : public Drawable, public Transformable, public Component, public ParticleSystemUpdate
{
public:
	int m_particleSpeed;
	ParticleSystem(unsigned int count, Color color, int speed = 50) : 
		m_particles(count),
		m_vertices(Points, count),
		m_lifetime(seconds(1.f)),
		m_emitter(0.f, 0.f),
		m_particleSpeed(speed),
		m_TargetTransform(nullptr)
	{
		Canvas::GetInstance()->AddDrawable(*this);
		for (size_t i = 0; i < m_particles.size(); ++i)
		{
			m_vertices[i].color = color; 
			ResetParticle(i); 
		}
	}

	void SetEmitterVector(Vector2f position)
	{
		m_emitter = position;
	}

	void SetEmitterTransform(Transformable& transform) override
	{
		m_TargetTransform = &transform;
	}

	void Update() override
	{
		for (size_t i = 0; i < m_particles.size(); ++i) // use async
		{
			// update the particle lifetime
			Particle& p = m_particles[i];

			// slightly works as before with the correct settings, tough realistikly I would never use it in a game
			// lifespan = milliseconds(lifespan.asMilliseconds() - Science->GetDeltaTimeMili()); // should use the physsics update time, but it wont compile with physics for some reason

			p.lifetime = milliseconds(p.lifetime.asMilliseconds() - Mono->GetDeltaTimeMili() * 5); /// TODO fix the lifetime, it seems to go down to fast so you never get to see the particles

			// if the particle is dead, respawn it
			if (p.lifetime <= Time::Zero)
				ResetParticle(i);

			// update the position of the corresponding vertex
			m_vertices[i].position += p.velocity * Mono->DeltaTime; // this might not be working as intended with delta instead of elapsed as seconds

			// update the alpha (transparency) of the particle according to its lifetime
			float ratio = p.lifetime.asSeconds() / m_lifetime.asSeconds();
			m_vertices[i].color.a = static_cast<Uint8>(ratio * 255);
		}
	}

private:
	virtual void draw(RenderTarget& target, RenderStates states) const 
	{
		target.draw(m_vertices);  /// TODO  particles should always be drawn in the top most layer, now that you mention it, should propably have a layer system for the canvas
	}

	struct Particle
	{
		Vector2f velocity;
		Time lifetime;
	};

	void ResetParticle(size_t index)
	{
		// give a random velocity and lifetime to the particle
		float angle = (rand() % 360) * 3.14f / 180.f;
		float speed = (float)(rand() % m_particleSpeed);
		m_particles[index].velocity = Vector2f(cos(angle) * speed, sin(angle) * speed);
		m_particles[index].lifetime = milliseconds((rand() % 2000) + 1000);

		// reset the position of the corresponding vertex
		if (m_TargetTransform) // would at some point want to get rid of checking this every frame, can't really see where it could go right now tough
		{
			m_vertices[index].position = m_TargetTransform->getPosition();
		}
		else
			m_vertices[index].position = m_emitter;
	}

	vector<Particle> m_particles;
	VertexArray m_vertices;
	Time m_lifetime;
	Transformable* m_TargetTransform;
	Vector2f m_emitter;
};
