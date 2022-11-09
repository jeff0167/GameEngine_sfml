#include "GameObject.h"
#include "Rigidbody.h"
#include "Physics.h"
#include <list>
#include <iostream>

using namespace sf;
using namespace std;

GameObject::GameObject()
{
	transform = Transformable();
	transform.setPosition(0, 0);
	transform.setRotation(0);
	transform.setScale(1, 1);
	transform.setOrigin(0, 0);
}

GameObject::GameObject(vector<Component*> _components)
{
	components = _components;
	CheckComponentType(_components);
	transform = Transformable();
	transform.setPosition(0, 0);
	transform.setRotation(0);
	transform.setScale(1, 1);
	transform.setOrigin(0, 0);
}

GameObject::GameObject(vector<Component*> _components, Transformable& _transform)
{
	components = _components;
	CheckComponentType(_components);
	transform = _transform;
}

void GameObject::CheckComponentType(vector<Component*> _components) // this will not work, only gets the component as a class type
{
	for (size_t i = 0; i < _components.size(); i++)
	{
		cout << typeid(_components[i]).name();
		CheckComponentType(*_components[i]);
	}
}

void GameObject::CheckComponentType(Component _component)
{
	cout << "hii";
	cout << typeid(_component).name(); // currently the component name is component, that explains a lot
	string s = typeid(_component).name(); // something to do with inline makes it not work

	if (s == "class Rigidbody")
	{
		cout << "Success";
		dynamic_cast<Rigidbody&>(_component).transform = &transform; // the gameobject has a transformable, inherited, but can we send it?
		Physics::GetInstance("MyFirstPhysicsSystem")->AddRigidbody(dynamic_cast<Rigidbody&>(_component));
	}
}

void GameObject::AddComponent(Component& _component) // if component is drawable then add it to the canvas
{
	cout << typeid(_component).name();
	string s = typeid(_component).name(); // something to do with inline makes it not work

	if (s == "class Rigidbody")
	{
		cout << "Success2";
		dynamic_cast<Rigidbody&>(_component).transform = &transform; // the gameobject has a transformable, inherited, but can we send it?
		Physics::GetInstance("MyFirstPhysicsSystem")->AddRigidbody(dynamic_cast<Rigidbody&>(_component));
	}
	//CheckComponentType(_component);
	components.push_back(&_component);
}

Component GameObject::GetComponent(Component& _component) // if component is drawable then add it to the canvas
{
	for (size_t i = 0; i < components.size(); i++)
	{
		if (components[i] == &_component)
		{
			return _component;
		}
	}
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