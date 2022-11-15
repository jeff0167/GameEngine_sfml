#pragma once
#include <SFML/Graphics.hpp>
#include "Component.h"

using namespace sf;
using namespace std;

class GameObject
{
public:
    GameObject() {};
	GameObject(Shape& drawShape);
	GameObject(Shape& drawShape, Component& _component);
	GameObject(Shape& drawShape, vector<Component*>& _components);

    Transformable* transform;
    vector<Component*> components; // you can only have one of each component type pr gameObject

	void AddComponent(Component& component);
    Component GetComponent(Component& component); 
	void RemoveComponent(Component& component);
    void CheckComponentType(Component& _component);
    void CheckComponentType(vector<Component*>& _components);

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
