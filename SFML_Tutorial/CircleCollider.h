#pragma once
#include <SFML/Graphics.hpp>
#include "Component.h"
#include "Collider.h"

using namespace sf;
using namespace std;

class CircleCollider : public Collider // collider needs to be inherited, like a circle collider or a box collider
{
public:
    float size = 1;

    CircleCollider() {}
    virtual bool Collision(Collider& _other);

    bool operator == (const CircleCollider& Ref) const
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
