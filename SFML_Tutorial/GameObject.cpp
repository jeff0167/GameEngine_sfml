#include "GameObject.h"
#include "Rigidbody.h"
#include "Physics.h"
#include "Canvas.h"
#include <iostream>

using namespace sf;
using namespace std;

GameObject::GameObject(Shape& drawShape)
{
	transform = &drawShape;
	Canvas::GetInstance("")->AddDrawable(drawShape);
}

GameObject::GameObject(Shape& drawShape, Component& _component)
{
	transform = &drawShape;
	Canvas::GetInstance("")->AddDrawable(drawShape);
	AddComponent(_component);
}

GameObject::GameObject(Shape& drawShape, vector<Component*>& _components)
{
	transform = &drawShape;
	Canvas::GetInstance("")->AddDrawable(drawShape);
	components = _components;
	CheckComponentType(components);
}

void GameObject::CheckComponentType(vector<Component*>& _components) // this will not work, only gets the component as a class type
{
	for (size_t i = 0; i < _components.size(); i++)
	{
		cout << typeid(_components[i]).name();
		CheckComponentType(*_components[i]);
	}
}

void GameObject::CheckComponentType(Component& _component)
{
	string s = typeid(_component).name();

	if (s == "class Rigidbody")
	{
		dynamic_cast<Rigidbody&>(_component).transform = transform;
		Physics::GetInstance("")->AddRigidbody(dynamic_cast<Rigidbody&>(_component));
	}
}

void GameObject::AddComponent(Component& _component)
{
	//cout << typeid(_component).name();
	string s = typeid(_component).name(); // something to do with inline makes it not work directly

	if (s == "class Rigidbody")
	{
		dynamic_cast<Rigidbody&>(_component).transform = transform;
		Physics::GetInstance("")->AddRigidbody(dynamic_cast<Rigidbody&>(_component)); // this feels so skethy my dude
	}
	else if (s == "class CircleCollider") // check if it contains collider in string
	{
		cout << "Added a collider pog";
		dynamic_cast<Collider&>(_component).transform = transform;
		Physics::GetInstance("")->AddCollider(dynamic_cast<Collider&>(_component));
	}
	components.push_back(&_component);
}

void GameObject::RemoveComponent(Component& _component)
{
	for (size_t i = 0; i < components.size(); i++)
	{
		if (components[i] == &_component)
		{
			components.erase(next(components.begin(), i), next(components.begin(), i + 1));
		}
	}
}

Component GameObject::GetComponent(Component& _component)
{
	for (size_t i = 0; i < components.size(); i++)
	{
		if (components[i] == &_component)
		{
			return _component;
		}
	}
}