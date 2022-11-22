#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Component;

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
	void RemoveComponent(Component& component);
    void CheckComponentType(Component& _component);
    void CheckComponentType(vector<Component*>& _components);

    template <typename T>
    T* GetComponent(T type);

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

    Component* GetComponentType(Component& component);
};
