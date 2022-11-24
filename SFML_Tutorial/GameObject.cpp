#include "GameObject.h"
#include "Rigidbody.h"
#include "Physics.h"
#include "Canvas.h"
#include "BoxCollider.h"
#include "CircleCollider.h"
#include "Debug.h"

using namespace sf;
using namespace std;

GameObject::GameObject(Shape& drawShape) :
	transform(&drawShape),
	components(vector<Component*>())
{
	Canvas::GetInstance("")->AddDrawable(drawShape);
}

GameObject::GameObject(Shape& drawShape, Component& _component) :
	transform(&drawShape)
{
	Canvas::GetInstance("")->AddDrawable(drawShape);
	AddComponent(_component);
}

GameObject::GameObject(Shape& drawShape, const vector<Component*>& _components) :
	transform(&drawShape)
{
	for (size_t i = 0; i < _components.size(); i++)
	{
		AddComponent(*_components[i]);
	}
	Canvas::GetInstance("")->AddDrawable(drawShape);
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

		auto cc = GetComponent(Collider()); // couldn't i get the collider and add it's rigidbody!?? but it says I can't dynamic cast a abstract class, cause it has a pure virtual func
		if(cc != nullptr) dynamic_cast<Collider&>(*cc).rigidbody = &dynamic_cast<Rigidbody&>(_component);

		Physics::GetInstance("")->AddRigidbody(dynamic_cast<Rigidbody&>(_component)); // this feels so skethy my dude
	}
	else if (classType == "class CircleCollider" || classType == "class BoxCollider") // check if it contains collider in string
	{
		dynamic_cast<Collider&>(_component).transform = transform; // need to figure out how to override existing libraries functions, find simple contains func for string and make it work in one line

		auto d = GetComponent(Rigidbody());
		dynamic_cast<Collider&>(_component).rigidbody = &dynamic_cast<Rigidbody&>(*d);

		Physics::GetInstance("")->AddCollider(dynamic_cast<Collider&>(_component)); // where the hell does it get the collider include from?!?
	}

	components.push_back(&_component);
	_component.gameObject = this; 
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


const vector<Component*>& GameObject::GetComponents()
{
	return components;
}