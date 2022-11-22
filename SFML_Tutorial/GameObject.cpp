#include "GameObject.h"
#include "Rigidbody.h"
#include "Physics.h"
#include "Canvas.h"
#include "Debug.h"

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
	string classType = typeid(_component).name(); // something to do with inline makes it not work directly

	if (classType == "class Rigidbody")
	{
		dynamic_cast<Rigidbody&>(_component).transform = transform;
		Physics::GetInstance("")->AddRigidbody(dynamic_cast<Rigidbody&>(_component)); // this feels so skethy my dude
	}
	else if (classType == "class CircleCollider") // check if it contains collider in string
	{
		Debug::GetInstance("")->Log("Added a Circle Collider");
		dynamic_cast<Collider&>(_component).transform = transform;
		Physics::GetInstance("")->AddCollider(dynamic_cast<Collider&>(_component));
	}
	else if (classType == "class BoxCollider") // check if it contains collider in string
	{
		Debug::GetInstance("")->Log("Added a Box Collider");
		dynamic_cast<Collider&>(_component).transform = transform;

		Physics::GetInstance("")->AddCollider(dynamic_cast<Collider&>(_component)); // where the hell does it get the collider include from?!?
	}
	components.push_back(&_component);
}

Component* GameObject::GetComponentType(Component& _component) 
{
	string classType = typeid(_component).name(); 

	if (classType == "class Rigidbody")
	{
		return &(dynamic_cast<Rigidbody&>(_component));
	}
	else if (classType == "class CircleCollider") 
	{
		return &(dynamic_cast<Collider&>(_component));
	}
	else if (classType == "class BoxCollider") 
	{
		return &(dynamic_cast<Collider&>(_component)); 
	}
	return nullptr;
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

template <typename T>
T* GameObject::GetComponent(T _component)
{
	for (size_t i = 0; i < components.size(); i++)
	{
		if (components[i] == _component)
		{
			 
		/*	string name = typeid(_component).name();

			Vector2f d(0, 0);

			if (name.find("Box") != string::npos)
			{
				d.x = 1;
			}

			return GetComponentType(_component);*/
			return components[i]; 
		}
	}
	return nullptr;
}