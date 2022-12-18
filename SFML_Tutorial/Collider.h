#pragma once
#include <SFML/Graphics.hpp>
#include "Component.h"
#include "Rigidbody.h"

using namespace sf;
using namespace std;

class Collider : public Component
{
public:
    Vector2f offsetPos = Vector2f(0,0); 
    Shape* shape;
    Rigidbody* rigidbody;
    float rotation;

    Collider() {};

    virtual bool Collision(Collider& _other) { return false; }; 

private:

};
