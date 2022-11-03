#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>
#include <iterator>
#include "GameObject.h"
#include <list>

using namespace sf;
using namespace std;

class Monobehaviour
{
public:
	static GameObject Instantiate(GameObject gameObject);
	static GameObject Instantiate(GameObject gameObject, Transformable transform);

private:

};
