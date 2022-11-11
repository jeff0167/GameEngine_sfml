#include "Monobehaviour.h"
#include "GameObject.h"
#include "Canvas.h"

using namespace sf;

GameObject Monobehaviour::Instantiate(GameObject gameObject)
{
	return gameObject;
}

GameObject Monobehaviour::Instantiate(GameObject gameObject, Transformable transform)
{
	return gameObject;
}