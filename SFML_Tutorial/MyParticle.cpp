#include "MyParticle.h"

float Umapi(float value, float istart, float istop, float ostart, float ostop)
{
	return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}

Particle::Particle(float radius, Texture& texture)
{
	dot.setRadius(radius);
	dot.setTexture(&texture);
	lifespan = 255;
}

Particle::~Particle()
{
}

void Particle::update() 
{
	if (isDead()) 
	{
		lifespan = 255; // also reset it 
	}
	if (lifespan == 255) 
	{
		position.x = 0;
		position.y = 0;

		startPos = GetTargetPos();
	}
	velocity += acceleration;
	position += velocity;
	dot.setPosition(position + startPos);

	acceleration = acceleration * 0.f;
	lifespan -= 1;
	dot.setFillColor(setColor(Umapi(lifespan, 255, 0, 360, 0), 1, 1));
}

void Particle::setPosition(float x, float y) 
{
	position.x = x;
	position.y = y;

	dot.setPosition(position);
}

void Particle::setVelocity(float x, float y) 
{
	velocity.x = x;
	velocity.y = y;
}

void Particle::SetParticleSystem(MyParticleSystem& ps) 
{
	m_particleSystem = &ps;
}

Vector2f Particle::GetTargetPos() 
{
	return m_particleSystem->GetTargetTransform();
}

void Particle::applyForce(Vector2f force) 
{
	acceleration += force;
}

Vector2f Particle::getVelocity() 
{
	return velocity;
}

bool Particle::isDead() 
{
	if (lifespan < 0) {
		return true;
	}
	else {
		return false;
	}
}

Color Particle::setColor(int hue, float sat, float val) 
{
	hue %= 360;
	while (hue < 0) hue += 360;

	if (sat < 0.f) sat = 0.f;
	if (sat > 1.f) sat = 1.f;

	if (val < 0.f) val = 0.f;
	if (val > 1.f) val = 1.f;

	int h = hue / 60;
	float f = float(hue) / 60 - h;
	float p = val * (1.f - sat);
	float q = val * (1.f - sat * f);
	float t = val * (1.f - sat * (1 - f));

	switch (h)
	{
	default:
	case 0:
	case 6: return sf::Color(val * 255, t * 255, p * 255);
	case 1: return sf::Color(q * 255, val * 255, p * 255);
	case 2: return sf::Color(p * 255, val * 255, t * 255);
	case 3: return sf::Color(p * 255, q * 255, val * 255);
	case 4: return sf::Color(t * 255, p * 255, val * 255);
	case 5: return sf::Color(val * 255, p * 255, q * 255);
	}
}

