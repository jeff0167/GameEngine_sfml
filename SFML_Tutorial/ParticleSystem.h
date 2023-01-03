#pragma once
#include <SFML/Graphics.hpp>
#include "Component.h"

using namespace sf;
using namespace std;

class ParticleSystem : public Drawable, public Transformable, public Component, public ParticleSystemUpdate
{
public:
	int m_particleSpeed;
	ParticleSystem(unsigned int count, Color color, int speed = 50);

	void SetEmitterVector(Vector2f position);

	void SetEmitterTransform(Transformable& transform) override;

	void Update() override;

private:
	virtual void draw(RenderTarget& target, RenderStates states) const;

	struct Particle
	{
		Vector2f velocity;
		Time lifetime;
	};

	void ResetParticle(size_t index);

	vector<Particle> m_particles;
	VertexArray m_vertices;
	Time m_lifetime;
	Transformable* m_TargetTransform;
	Vector2f m_emitter;
};
