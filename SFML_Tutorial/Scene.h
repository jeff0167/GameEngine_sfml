#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"

#define Hiearchy Scene::GetInstance()

using namespace sf;
using namespace std;

class Scene 
{
public:
    Scene(Scene& other) = delete;

    void operator=(const Scene&) = delete;

    static Scene* GetInstance();

    vector<GameObject*> m_GameObjects;

    void DebugInfo();
    void AddGameObject(GameObject& drawable);
    void RemoveGameObject(GameObject& _drawable);
    const vector<GameObject*>& GetGameObjects();

    void DisplaySceneWindow(RenderWindow& window);

protected:
    static Scene* _Scene;

    Scene() {};

};
