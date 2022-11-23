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
	components = vector<Component*>();
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

		auto s = GetComponents();
		auto d = GetComponent(s, Rigidbody());
		dynamic_cast<Collider&>(_component).rb = dynamic_cast<Rigidbody*>(d);

		Physics::GetInstance("")->AddCollider(dynamic_cast<Collider&>(_component));
	}
	else if (classType == "class BoxCollider") // check if it contains collider in string
	{
		Debug::GetInstance("")->Log("Added a Box Collider");
		dynamic_cast<Collider&>(_component).transform = transform;

		auto s = GetComponents();
		auto d = GetComponent(s, Rigidbody());
		dynamic_cast<Collider&>(_component).rb = dynamic_cast<Rigidbody*>(d);

		//Debug::GetInstance("")->Log(dynamic_cast<Collider&>(_component).rb->Magnitude()); // gets added, i suppose, just not alloweed to read it right now, can't remember why things are sometimes not allowed to be read

		Physics::GetInstance("")->AddCollider(dynamic_cast<Collider&>(_component)); // where the hell does it get the collider include from?!?
	}
	components.push_back(&_component);
	Debug::GetInstance("")->Log(to_string(GetComponents().size())); // the game object is being set correctly
	_component.gameObject = this; // im a buqing genius ^^, actually i'm not, i totaly over saw that i didn't set the gameobject and had other issues which didn't directly link me what the issue was, leading to hours of try and error until i realized the problem right here :)
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


vector<Component*> GameObject::GetComponents()
{
	//if (components.empty()) return vector<Component*>(); // we will never return null

	return components;
}