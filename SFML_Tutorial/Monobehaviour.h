#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"

using namespace sf;
using namespace std;

class Monobehaviour
{
public:
	Monobehaviour(Monobehaviour& other) = delete; 

	void operator=(const Monobehaviour&) = delete;

	static Monobehaviour* GetInstance();

	static GameObject Instantiate(GameObject gameObject);
	static GameObject Instantiate(GameObject gameObject, Transformable transform);

	static void Destroy(GameObject* gameObject);

	static void Invoke(function<void()> function, float callDelay);

	float DeltaTime;

private:
	static void DelayedCall(function<void()> function, float callDelay);

	static Monobehaviour* _monobehaviour;

	Monobehaviour() {};
};
