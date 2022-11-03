#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>
#include <iterator>
#include <list>
#include "Component.h"

using namespace sf;
using namespace std;

class GameObject
{
public:
	GameObject();
	GameObject(list<Component> components);
	GameObject(list<Component> _components, Transformable _transform);

    Transformable transform;
    list<Component> components;
	void AddComponent(Component component);
	void RemoveComponent(Component component);

    bool operator == (const GameObject& Ref) const
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
