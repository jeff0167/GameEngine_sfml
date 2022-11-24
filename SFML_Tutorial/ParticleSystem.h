#pragma once
#include <SFML/Graphics.hpp>
#include "Canvas.h"

using namespace sf;
using namespace std;

class ParticleSystem : public Drawable, public Transformable
{
public:
	int m_particleSpeed;
	ParticleSystem(unsigned int count, Color color, int speed = 50) : // a particle is literally only a point, which is just a pixel and can't just be increased in size
		m_particles(count),
		m_vertices(Points, count),
		m_lifetime(seconds(3.f)),
		m_emitter(0.f, 0.f),
		m_particleSpeed(speed)
	{
		Canvas::GetInstance("")->AddDrawable(*this);
		for (size_t i = 0; i < m_particles.size(); ++i)
		{
			m_vertices[i].color = color; // couldn't you somehow set the color once!??
			resetParticle(i); // this tough needs to be called
		}
	}

	void setEmitter(::Vector2f position)
	{
		m_emitter = position;
	}

	void update(Time elapsed)
	{
		for (size_t i = 0; i < m_particles.size(); ++i)
		{
			// update the particle lifetime
			Particle& p = m_particles[i];
			p.lifetime -= elapsed;

			// if the particle is dead, respawn it
			if (p.lifetime <= Time::Zero)
				resetParticle(i);

			// update the position of the corresponding vertex
			m_vertices[i].position += p.velocity * elapsed.asSeconds();

			// update the alpha (transparency) of the particle according to its lifetime
			float ratio = p.lifetime.asSeconds() / m_lifetime.asSeconds();
			m_vertices[i].color.a = static_cast<Uint8>(ratio * 255);
		}
	}

private:
	virtual void draw(RenderTarget& target, RenderStates states) const
	{
		// apply the tranorm
		states.transform *= getTransform();

		// our particles don't use a texture
	   // states.texture = NULL;

		// draw the vertex array
		target.draw(m_vertices, states);
	}

private:
	struct Particle
	{
		Vector2f velocity;
		Time lifetime;
	};

	void resetParticle(size_t index)
	{
		// give a random velocity and lifetime to the particle
		float angle = (rand() % 360) * 3.14f / 180.f;
		float speed = (rand() % m_particleSpeed);
		m_particles[index].velocity = Vector2f(cos(angle) * speed, sin(angle) * speed);
		m_particles[index].lifetime = milliseconds((rand() % 2000) + 1000);

		// reset the position of the corresponding vertex
		m_vertices[index].position = m_emitter;
	}

	vector<Particle> m_particles;
	VertexArray m_vertices;
	Time m_lifetime;
	Vector2f m_emitter;
};
