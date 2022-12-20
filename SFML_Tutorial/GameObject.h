#pragma once
#include <SFML/Graphics.hpp>
#include "Debug.h"

using namespace sf;
using namespace std;

class Component;

class GameObject
{
protected:

public:
	GameObject() {};
	~GameObject();
	GameObject(Text& drawShape);
	GameObject(Shape& drawShape);
	GameObject(Shape& drawShape, Component& _component);
	GameObject(Shape& drawShape, Component& _component, Component& _component2);
	GameObject(Shape& drawShape, Component& _component, Component& _component2, Component& _component3);
	GameObject(Shape& drawShape, const vector<Component*>& _components);

	Transformable* transform; // suppose you could even use this as an id, would really just want to use the memory adress of the gameobject as an identifier, but no, that is terrible hard
	vector<Transformable*> transformers;
	vector<Component*> components; 

	void AddComponent(Component& component);
	void RemoveComponent(Component& component);
	const vector<Component*>& GetComponents();

	template <class T>
	Component* GetComponent(T type)
	{
		for (auto& component : this->components)
		{
			if (typeid(component).name() == typeid(type).name()) // *this->components[i]).name(), remember the * in front!!
			{
				return component;
			}
		}

		for (auto& component : this->components) // is this even necessarcy!? ok say you just want a collider, not a specifik one, then you would do this, yes
		{
			string s = typeid(component).name();
			if (s.find("Collider") != string::npos) return component;  // specificly for collider, very bad, dont do
		}

		return nullptr;
	}

	Vector2f MyPos;

	string ToString();

private:
	Component* GetComponentType(Component& component);

};
