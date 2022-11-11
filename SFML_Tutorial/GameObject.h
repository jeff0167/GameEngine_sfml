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
    GameObject() {};
	GameObject(Shape& drawShape);
	GameObject(vector<Component*> _components, Transformable& _transform);

    Transformable* transform;
    vector<Component*> components;

	void AddComponent(Component& component);
    Component GetComponent(Component& component); // you can only have one component type pr gameobject
	void RemoveComponent(Component& component);
    void CheckComponentType(Component _component);
    void CheckComponentType(vector<Component*> _components);

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
