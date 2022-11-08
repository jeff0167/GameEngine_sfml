#include "GameObject.h"

using namespace sf;

GameObject::GameObject()
{
	transform = Transformable();
	transform.setPosition(0, 0);
	transform.setRotation(0);
	transform.setScale(1, 1);
	transform.setOrigin(0, 0);
}

GameObject::GameObject(list<Component> _components)
{
	components = _components;
	transform = Transformable();
	transform.setPosition(0, 0);
	transform.setRotation(0);
	transform.setScale(1, 1);
	transform.setOrigin(0, 0);
}

GameObject::GameObject(list<Component> _components, Transformable _transform)
{
	components = _components;
	transform = _transform;
}

void GameObject::AddComponent(Component _component) // if component is drawable then add it to the canvas
{
	components.push_back(_component);
}

void GameObject::RemoveComponent(Component _component)
{
	for (Component component : components)
	{
		if (component == _component) 
		{
			components.remove(component);
		}
	}
}