#pragma once
#include <SFML/Graphics.hpp>
#include "Component.h"
#include "Rigidbody.h"

using namespace sf;
using namespace std;

class Collider : public Component
{
public:
    Vector2f offsetPos = Vector2f(0,0); // would maybe have to cach rigidbody, instead of getting the reference through the gameobject and getcomponent
   // Transformable* transform;
    Shape* s;
    Rigidbody* rigidbody;
    float rotation;

    Collider() {};

    virtual bool Collision(Collider& _other) { return false; };  // set virtual func equal to 0 to make it a pure virtual/abstract func

private:

};
