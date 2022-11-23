#pragma once
#include <SFML/Graphics.hpp>
#include "Component.h"

using namespace sf;
using namespace std;

class EmptyComponent : public Component
{
public:

    Transformable* transform;

    EmptyComponent() {}

    bool operator == (const EmptyComponent& Ref) const
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
