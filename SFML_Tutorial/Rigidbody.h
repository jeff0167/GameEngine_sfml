#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>
#include <iterator>
#include <list>
#include "Component.h"

using namespace sf;
using namespace std;

class Rigidbody : public Component 
{
public:
    Vector2f velocity;
    Vector2f rotationalVelocity;
    Transformable* transform;

    Rigidbody() {}
    void AddForce();

    bool operator == (const Rigidbody& Ref) const
    {
        return(this->Member == Ref.GetMember());
    }

    const int GetMember() const
    {
        return(this->Member);
    }

private:
    int Member;
};
