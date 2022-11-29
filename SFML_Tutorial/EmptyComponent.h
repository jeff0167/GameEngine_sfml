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

private:

};
