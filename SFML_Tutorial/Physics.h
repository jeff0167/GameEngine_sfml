#pragma once
#include <SFML/Graphics.hpp>
#include "Rigidbody.h"
#include "Collider.h"

using namespace sf;
using namespace std;

class Physics // needs to be a singleton and have an observer pattern
{
public:
	void Physcis(Physics& other) = delete; // Singletons should not be assignable

	void operator=(const Physics&) = delete;

	// This is the static method that controls the access to the singleton
	// instance. On the first run, it creates a singleton object and places it
	// into the static field. On subsequent runs, it returns the client existing
	// object stored in the static field

	static Physics* GetInstance(const string& value);

	string value() const {
		return value_;
	}

	vector<Rigidbody*> rigidbodies;
	vector<Rigidbody*> GetRigidbodies();
	void AddRigidbody(Rigidbody& _rigidbody);
	void RemoveRigidbody(Rigidbody& _rigidbody);
	void PhysicsUpdate();

	vector<Collider*> colliders; // need to use dynamic programming when checking for collisions, though for a simple test with only a handfull of colliders it will do fine
	vector<Collider*> GetColliders();
	void AddCollider(Collider& _collider);
	void RemoveCollider(Collider& _collider);
	void PhysicsCollisionUpdate();

protected:
	string value_;
	static Physics* _physics;

	Physics(const string& value) : value_(value) {}; // constructor

};
