#include <SFML/Graphics.hpp>
#include "Scene.h"
#include "Pch.h"
#include "Animation.h"
#include "Canvas.h"
#include "Scene.h"
#include "Physics.h"
#include "Component.h"
#include "Monobehaviour.h"
#include "ParticleSystem.h"
#include "CircleCollider.h"
#include "Input.h"
#include "Mathf.h"
#include "Debug.h"
#include "Rigidbody.h"
#include "MyParticle.h"

using namespace sf;
using namespace std;

static void Draw();
static void MouseCreation();

RenderWindow* window;

GameObject circle;

Scene* Scene::_Scene = nullptr;

Scene* Scene::GetInstance()
{
	if (_Scene == nullptr) {
		_Scene = new Scene();
	}
	return _Scene;
}

void Scene::AddGameObject(GameObject& gameObject)
{
	m_GameObjects.push_back(&gameObject);
}

void Scene::RemoveGameObject(GameObject& gameObject)
{
	for (size_t i = 0; i < m_GameObjects.size(); i++)
	{
		if (m_GameObjects[i] == &gameObject)
		{
			m_GameObjects.erase(next(m_GameObjects.begin(), i), next(m_GameObjects.begin(), i + 1));
		}
	}
}

const vector<GameObject*>& Scene::GetGameObjects()
{
	return m_GameObjects;
}

void Scene::DisplaySceneWindow(RenderWindow& _window)
{
	window = &_window;

	CircleCollider circle2 = CircleCollider(*new CircleShape(50, 50), Vector2f(100, 200), Color::Red);
	circle = GameObject(*circle2.shape, circle2, *new Rigidbody());

	window->setFramerateLimit(120); 
	while (window->isOpen()) 
	{
		Mono->UpdateTime();

		//DebugFrameRate(Mono->_time);
		Event _event;
		while (window->pollEvent(_event))
		{
			switch (_event.type)
			{
			case Event::Closed:
				window->close();
				break;
			case Event::Resized:
				printf("New window width: %i New window heigth: %i \n", _event.size.width, _event.size.height);
			case Event::TextEntered:
				if (_event.text.unicode < 128) {
					printf("%c", _event.text.unicode);
				}
				break;
			}
		}

		MouseCreation();

		Draw();
	}
}

static void MouseCreation() 
{
	if (Mouse::isButtonPressed(Mouse::Left)) // check all clickable objects for their pos, oh no, another thing that needs to keep track of a list of objects
	{
		DebugLog("clicked");
		Vector2i mousePos = Mouse::getPosition(*window);
		circle.transform->setPosition((mousePos.x), (mousePos.y));
	}
	if (Mouse::isButtonPressed(Mouse::Right)) // spawn menu at mouse pos, here you can make a gameObject
	{
		
	}
}

static void Draw() 
{
	window->clear(Color(255, 204, 92));
	Renderer->DrawCanvas();
	window->display();
}

void Scene::DebugInfo()
{	
	// write debug info in game/scene window
}