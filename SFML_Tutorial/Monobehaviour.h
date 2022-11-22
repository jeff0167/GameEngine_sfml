#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"

using namespace sf;
using namespace std;

class Monobehaviour
{
public:
	static GameObject Instantiate(GameObject gameObject);
	static GameObject Instantiate(GameObject gameObject, Transformable transform);

private:

};
