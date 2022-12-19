#include <SFML/Graphics.hpp>
//#include "Scene.h"
#include "Pch.h"
#include "Animation.h"
#include "Canvas.h"
#include "SceneWindow.h"
#include "Physics.h"
#include "Component.h"
#include "Monobehaviour.h"
#include "ParticleSystem.h"
#include "CircleCollider.h"
#include "Input.h"
#include "Mathf.h"
#include "Debug.h"
#include "Rigidbody.h"
#include "MousePoint.h"
#include "MyParticle.h"
#include <sstream>

#include "../yaml-cpp/include/yaml-cpp/yaml.h"

using namespace sf;
using namespace std;

static void Draw();
static void MouseCreation();
static void CreateDefaultSceneWindowObjects();

RenderWindow* window;

Text text;
Font font;

bool m_DisplayMenu = false;

SceneWindow* SceneWindow::_SceneWindow = nullptr;

SceneWindow* SceneWindow::GetInstance()
{
	if (_SceneWindow == nullptr) {
		_SceneWindow = new SceneWindow();
	}
	return _SceneWindow;
}

void SceneWindow::AddGameObject(GameObject& gameObject)
{
	m_GameObjects.push_back(&gameObject);
}

void SceneWindow::RemoveGameObject(GameObject& gameObject)
{
	for (size_t i = 0; i < m_GameObjects.size(); i++)
	{
		if (m_GameObjects[i] == &gameObject)
		{
			m_GameObjects.erase(next(m_GameObjects.begin(), i), next(m_GameObjects.begin(), i + 1));
		}
	}
}

const vector<GameObject*>& SceneWindow::GetGameObjects()
{
	return m_GameObjects;
}

// suppose the sceneWindow should hold an instance of the current scene to be manipulated with and can be changed out
// the sandbox would need to tell which scene needs to be loaded
// a scene class may or may not have a scene text file to load
// suppose you could make a instance of a scene and tell it/ write the code of what it should hold
// the scene would then be your work space
// you somehow need a entry point into the gameloop?
// or perhaps the gameloop already has all it's functions that it needs and you just subscribe things to those events to be called

void SceneWindow::DisplaySceneWindow(RenderWindow& _window)
{
	window = &_window;
	CreateDefaultSceneWindowObjects();

	//CircleCollider circle2 = CircleCollider(*new CircleShape(50, 50));
	//m_circle = GameObject(*circle2.shape, circle2, *new Rigidbody());

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

// as a developer you would just say on mouse event and that would 

static void MouseCreation()
{
	YAML::Emitter out;
	out << YAML::BeginMap;
	Vector2f mousePos = (Vector2f)Mouse::getPosition(*window); // can't i manually set it so you don't ever have to touch it!??!
	MousePos.setPosition(mousePos.x, mousePos.y); 
	if (Mouse::isButtonPressed(Mouse::Left)) // check all clickable objects for their pos, oh no, another thing that needs to keep track of a list of objects
	{
		if (text.getGlobalBounds().intersects(MousePos.getGlobalBounds()))
		{
			MyScene->SaveScene();
			DebugLog("Clicked on menu"); // now do what the text tells you!
		}
		m_DisplayMenu = false;
		//m_circle.transform->setPosition((mousePos.x), (mousePos.y));
	}
	if (Mouse::isButtonPressed(Mouse::Right)) // spawn menu at mouse pos, here you can make a gameObject
	{
		m_DisplayMenu = true;
		text.setPosition((mousePos.x), (mousePos.y));
	}
	if (!m_DisplayMenu)
	{
		text.setPosition((mousePos.x) + 10000, (mousePos.y) + 10000);
	}
}

static void Draw()
{
	window->clear(Color(255, 204, 92));
	Renderer->DrawCanvas();
	window->display();
}

void SceneWindow::DebugInfo()
{
	// write debug info in game/Scene window
}

void SceneWindow::LoadScene()
{
	// we load the curent scene
	// we take all it's objects and add them?!?
	// well if they are created within the scene or perhaps sceneSerializer then that will create the objects and the objects when created are automaticly added to the scenemanager hiearchy
	// so the scene just need to deserialize and create the objects
	m_Scene->LoadScene();
}

void SceneWindow::LoadScene(string scenePath)
{
	m_Scene->LoadScene(scenePath);
}

void SceneWindow::SetActiveScene(string scenePath)
{
	Scene::GetInstance()->SetActiveScene(scenePath);
}

static void CreateDefaultSceneWindowObjects()
{
	if (!font.loadFromFile("BebasNeue-Regular.ttf"))
	{
		DebugLog("Could not load font");
	}

	text.setString("Menu");
	text.setCharacterSize(50); // in pixels, not points!
	text.setOrigin((text.getString().getSize() * text.getCharacterSize()) / 5.0f, text.getCharacterSize() / 2); // roughly the center
	text.setFont(font);
	text.setFillColor(Color::Black);

	Renderer->AddDrawable(text);
}