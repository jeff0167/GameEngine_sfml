#include "Scene.h"

using namespace sf;
using namespace std;

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

void Scene::DebugInfo()
{	
	// write debug info in game/scene window
}