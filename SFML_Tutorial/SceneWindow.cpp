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
#include "MyParticle.h"
#include <sstream>

using namespace sf;
using namespace std;

static void Draw();
static void SaveSceneWindow();
static void LoadSceneWindow();
static void MouseCreation();
static void CreateGameObject();
static void CreateDefaultSceneWindowObjects();

RenderWindow* window;

BoxCollider mousePixel;
GameObject m_circle, m_mouse;
Text text, otherText;
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

void SceneWindow::DisplaySceneWindow(RenderWindow& _window)
{
	window = &_window;
	CreateDefaultSceneWindowObjects();

	CircleCollider circle2 = CircleCollider(*new CircleShape(50, 50));
	m_circle = GameObject(*circle2.shape, circle2, *new Rigidbody());
	LoadSceneWindow();

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
			SaveSceneWindow();
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

void SceneWindow::DebugInfo()
{
	// write debug info in game/Scene window
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

static void SaveSceneWindow() // if you use new you MUST call delete, new allocates an memory adress on the heap/stack,  it's slower but has it's uses 
{
	ofstream file_obj("Scene01.txt", std::ofstream::trunc); // trunc and no ios::app will clear all contents of the file before writing

	//for (size_t i = 0; i < Hiearchy->GetGameObjects().size(); i++)
	//{
	//	file_obj.write((char*)&Hiearchy->GetGameObjects()[i], sizeof(&Hiearchy->GetGameObjects()[i]));
	//}
	Vector2f mousePos = (Vector2f)Mouse::getPosition(*window);
	string name = to_string(mousePos.x) + "," + to_string(mousePos.y);

	file_obj << name.c_str();

	//file_obj.close();
}

vector<string> split(string s, string delimiter) {
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	string token;
	vector<string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(pos_start));
	return res;
}

static void LoadSceneWindow()
{
	ifstream file_obj("Scene01.txt", ios::in);
	stringstream s;
	s << file_obj.rdbuf();

	string val = s.str();
	string delimiter = ",";

	vector<string> values = split(val, delimiter);
	
	float x = stof(values[0]);
	float y = stof(values[1]);

	DebugLog(x);
	DebugLog(y);
	m_circle.transform->setPosition(x, y);

	file_obj.close();
}
