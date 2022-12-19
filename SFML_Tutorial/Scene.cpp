#include <sstream>
#include "Scene.h"
#include "SceneWindow.h"
#include "MousePoint.h"
#include "CircleCollider.h"
#include "Monobehaviour.h"
#include "Pch.h"

Scene* Scene::_Scene = nullptr;

Scene* Scene::GetInstance()
{
	if (_Scene == nullptr)
	{
		_Scene = new Scene();
	}
	return _Scene;
}

vector<string> split(string s, string delimiter)
{
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	string token;
	vector<string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != string::npos)
	{
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(pos_start));
	return res;
}

void Scene::LoadScene()
{
	ifstream file_obj(MyScene->m_ScenePath + ".txt", ios::in);
	stringstream s;
	s << file_obj.rdbuf();

	string val = s.str();
	string delimiter = ",";

	vector<string> values = split(val, delimiter);

	float x = stof(values[0]);
	float y = stof(values[1]);

	DebugLog(x);
	DebugLog(y);

	CircleCollider circle2 = CircleCollider(*new CircleShape(50, 50));
	GameObject m_circle = GameObject(*circle2.shape, circle2, *new Rigidbody());
	m_circle.transform->setPosition(x, y);

	file_obj.close();
}

void Scene::LoadScene(string scenePath) // well okey, we load the text file but we do nothing with it?!?! at least return it or put it into the scene
{
	// well suppose the scene just holds all gameObjects and we try and create them by reading from the scene we load

	/*ifstream file_obj(scenePath + ".txt", ios::in);
	stringstream s;
	s << file_obj.rdbuf();

	file_obj.close();*/

	ifstream file_obj("Scene01.txt", ios::in); // deserializing
	stringstream s;
	s << file_obj.rdbuf();

	string val = s.str();
	string delimiter = ",";

	vector<string> values = split(val, delimiter);

	float x = stof(values[0]);
	float y = stof(values[1]);

	DebugLog(x);
	DebugLog(y);

	file_obj.close();
}

void Scene::SaveScene() 
{
	ofstream file_obj(MyScene->m_ScenePath + ".txt", std::ofstream::trunc); // trunc and no ios::app will clear all contents of the file before writing

	/*for (size_t i = 0; i < SceneManager->GetGameObjects().size(); i++)
	{
		file_obj.write((char*)&SceneManager->GetGameObjects()[i], sizeof(&SceneManager->GetGameObjects()[i]));
	}*/
	Vector2f mousePos = MousePos.getPosition();
	string name = to_string(mousePos.x) + "," + to_string(mousePos.y);

	file_obj << name.c_str();

	file_obj.close();
}

void Scene::SetActiveScene(string& scenePath)
{
	m_ScenePath = scenePath;
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

void Scene::InitializeSceneGameObjects()
{
	return;
}
