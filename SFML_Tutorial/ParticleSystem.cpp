#include "ParticleSystem.h"
#include "Canvas.h"
#include "Monobehaviour.h"

ParticleSystem::ParticleSystem(unsigned int count, Color color, int speed):
m_particles(count),
m_vertices(Points, count),
m_lifetime(seconds(1.f)),
m_emitter(0.f, 0.f),
m_particleSpeed(speed),
m_TargetTransform(nullptr)
{
	Renderer->AddDrawable(*this);
	for (size_t i = 0; i < m_particles.size(); ++i)
	{
		m_vertices[i].color = color;
		ResetParticle(i);
	}
}

void ParticleSystem::SetEmitterVector(Vector2f position)
{
	m_emitter = position;
}

void ParticleSystem::SetEmitterTransform(Transformable& transform)
{
	m_TargetTransform = &transform;
}

void ParticleSystem::Update()
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

void ParticleSystem::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(m_vertices);
}

void ParticleSystem::ResetParticle(size_t index)
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
