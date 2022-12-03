#include "GameObject.h"
#include "Rigidbody.h"
#include "Physics.h"
#include "Canvas.h"
#include "Collider.h"
#include "Debug.h"
#include <type_traits>
#include <typeinfo>

template <typename T> class remove_all_pointers {
public:
	typedef T type;
};

template <typename T> class remove_all_pointers<T*> {
public:
	typedef typename remove_all_pointers<T>::type type;
};

using namespace sf;
using namespace std;

GameObject::GameObject(Shape& drawShape) :
	transform(&drawShape),
	components(vector<Component*>())
{
	Canvas::GetInstance()->AddDrawable(drawShape);
}

GameObject::GameObject(Shape& drawShape, Component& _component) :
	transform(&drawShape)
{
	Canvas::GetInstance()->AddDrawable(drawShape);
	AddComponent(_component);
}

GameObject::GameObject(Shape& drawShape, Component& _component, Component& _component2) :
	transform(&drawShape)
{
	Canvas::GetInstance()->AddDrawable(drawShape);
	AddComponent(_component);
	AddComponent(_component2);
}

GameObject::GameObject(Shape& drawShape, Component& _component, Component& _component2, Component& _component3) :
	transform(&drawShape)
{
	Canvas::GetInstance()->AddDrawable(drawShape);
	AddComponent(_component);
	AddComponent(_component2);
	AddComponent(_component3);
}

GameObject::GameObject(Shape& drawShape, const vector<Component*>& _components) :
	transform(&drawShape)
{
	for (size_t i = 0; i < _components.size(); i++)
	{
		AddComponent(*_components[i]);
	}
	Canvas::GetInstance()->AddDrawable(drawShape);
}

void GameObject::AddComponent(Component& _component)
{
	//Debug::GetInstance()->Log(typeid(_component).name());
	string classType = typeid(_component).name(); 

	if (classType == "class Rigidbody")
	{
		dynamic_cast<Rigidbody&>(_component).transform = transform; // Must manually cast by each class type, super anoying

		auto cc = GetComponent(Collider()); 
		if (cc != nullptr) dynamic_cast<Collider&>(*cc).rigidbody = &dynamic_cast<Rigidbody&>(_component);

		Physics::GetInstance()->AddRigidbody(dynamic_cast<Rigidbody&>(_component));
	}
	else if (classType == "class CircleCollider" || classType == "class BoxCollider") // check if it contains collider in string
	{
		auto d = GetComponent(Rigidbody());
		dynamic_cast<Collider&>(_component).rigidbody = &dynamic_cast<Rigidbody&>(*d);

		Physics::GetInstance()->AddCollider(dynamic_cast<Collider&>(_component)); 
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
	else if (classType == "class CircleCollider" || classType == "class BoxCollider")
	{
		return &(dynamic_cast<Collider&>(_component));
	}
	return nullptr;
}

void GameObject::RemoveComponent(Component& _component) // sure it's no longer in it's list but the component still exist in memory and has shares the same info with gameObject as before
{
	string removeComponentType = typeid(_component).name();
	for (size_t i = 0; i < components.size(); i++) // it needs to use templates as well to remove components by type and certainly not purely by the memory adress
	{
		string currentComponentType = typeid(*components[i]).name();
		//Debug::GetInstance()->Log("to remove: " + removeComponentType + " current: " + currentComponentType);
		if (removeComponentType == currentComponentType) 
		{
			components.erase(next(components.begin(), i), next(components.begin(), i + 1));
		}
	}
}

const vector<Component*>& GameObject::GetComponents()
{
	return components;
}

GameObject::~GameObject()
{
	// gameObject should delete all it's related components and references
}