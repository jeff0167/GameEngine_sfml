#pragma once
#include <SFML/Graphics.hpp>
#include "Debug.h"

using namespace sf;
using namespace std;

class Component;

class GameObject
{
public:
	GameObject() {};
	GameObject(Shape& drawShape);
	GameObject(Shape& drawShape, Component& _component);
	GameObject(Shape& drawShape, const vector<Component*>& _components);

	Transformable* transform; // suppose you could even use this as an id, would really just want to use the memory adress of the gameobject as an identifier, but no, that is terrible hard
	vector<Component*> components; // you can only have one of each component type pr gameObject

	void AddComponent(Component& component);
	void RemoveComponent(Component& component);
	void CheckComponentType(Component& _component);
	void CheckComponentType(vector<Component*>& _components);

	const vector<Component*>& GetComponents();

	template <class T>
	Component* GetComponent(T type) // vector<Component*>& _components,   the param before T
	{
		for (size_t i = 0; i < this->components.size(); i++)
		{
			if (typeid(*this->components[i]).name() == typeid(type).name())      //*this->components[i]).name()       remember the * in front!!
			{
				return this->components[i];
			}
		}

		for (size_t i = 0; i < this->components.size(); i++)
		{
			string s = typeid(*this->components[i]).name();
			if (s.find("Collider") != string::npos) return this->components[i]; // specificly for collider, very bad, dont do
		}

		return nullptr;
	}

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
