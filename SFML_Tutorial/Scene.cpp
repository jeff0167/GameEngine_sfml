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
static void SaveScene();
static void LoadScene();
static void MouseCreation();
static void CreateGameObject();
static void CreateDefaultSceneObjects();

RenderWindow* window;

BoxCollider mousePixel;
GameObject m_circle, m_mouse;
Text text, otherText;
Font font;

bool m_DisplayMenu = false;

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
	LoadScene();
	CreateDefaultSceneObjects();

	CircleCollider circle2 = CircleCollider(*new CircleShape(50, 50));
	m_circle = GameObject(*circle2.shape, circle2, *new Rigidbody());

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
	Vector2f mousePos = (Vector2f)Mouse::getPosition(*window);
	m_mouse.transform->setPosition(mousePos.x, mousePos.y);
	if (Mouse::isButtonPressed(Mouse::Left)) // check all clickable objects for their pos, oh no, another thing that needs to keep track of a list of objects
	{
		if (text.getGlobalBounds().intersects(mousePixel.shape->getGlobalBounds()))
		{
			CreateGameObject();
			DebugLog("Clicked on menu"); // now do what the text tells you!
			SaveScene();
		}
		m_DisplayMenu = false;
		m_circle.transform->setPosition((mousePos.x), (mousePos.y));
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

void Scene::DebugInfo()
{
	// write debug info in game/scene window
}

static void CreateDefaultSceneObjects()
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
	Renderer->AddDrawable(otherText);

	mousePixel = BoxCollider(*new RectangleShape(Vector2f(1, 1)));
	m_mouse = GameObject(*mousePixel.shape, mousePixel, *new Rigidbody());
}

static void CreateGameObject()
{
	CircleShape* s = new CircleShape(10, 50);
	s->setPosition(m_mouse.transform->getPosition());
	s->setOrigin(m_mouse.transform->getOrigin());
	s->setFillColor(Color::Blue);

	GameObject g = GameObject(*s);
}

static void SaveScene() // if you use new you MUST call delete, new allocates an memory adress on the heap/stack,  it's slower but has it's uses 
{
	ofstream file_obj("Scene01.txt", ios::app);

	//for (size_t i = 0; i < Hiearchy->GetGameObjects().size(); i++)
	//{
	//	file_obj.write((char*)&Hiearchy->GetGameObjects()[i], sizeof(&Hiearchy->GetGameObjects()[i]));
	//}

	char obj[20] = "14";
	file_obj.write("14", sizeof(obj));

	file_obj.close();
}

static void LoadScene()
{
	//ofstream file_obj2("Scene01.txt", ios::app);

	//file_obj2 << " " << 3 << " " << 7 << " " << 64 << " " << 69 << " ";
	//file_obj2.close();

	ifstream file_obj("Scene01.txt", ios::in);

	char obj[3] = "14";

	//float x1, x2, x3, x4;
	//file_obj >> x1 >> x2 >> x3 >> x4; // way to clumsy

	//DebugLog(x4);
	//DebugLog(x3);
	//DebugLog(x2);
	//DebugLog(x1);

	while (!file_obj.eof()) { // don't know why this doesn't work atm, need to figure out references and how to safe them
		// Checking further
		file_obj.read((char*)&obj, sizeof(obj));
	}

	DebugLog(obj);

	//otherText = Text();
	//otherText.setString(obj.getString());
	//otherText.setCharacterSize(obj.getCharacterSize()); 
	//otherText.setOrigin((obj.getString().getSize() * obj.getCharacterSize()) / 5.0f, obj.getCharacterSize() / 2); // roughly the center
	//otherText.setFont(font);
	//otherText.setFillColor(obj.getFillColor());

	file_obj.close();
}
