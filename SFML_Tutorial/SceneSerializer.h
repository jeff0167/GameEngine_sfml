#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "Scene.h"

using namespace sf;
using namespace std;

class SceneSerializer
{
public:
	SceneSerializer() {};

	string SerializeScene(Scene& scene);
	void DeSerializeScene(string& yaml);

};

