#pragma once
#include <SFML/Graphics.hpp>
#include "Component.h"

using namespace sf;
using namespace std;

class Collider : public Component 
{
public:
    Vector2f offsetPos = Vector2f(0,0);
    Transformable* transform;
    float rotation;

    Collider() {}
    virtual bool Collision(Collider& _other) = 0;  // set virtual func equal to 0 to make it a pure virtual/abstract func

    bool operator == (const Collider& Ref) const
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
