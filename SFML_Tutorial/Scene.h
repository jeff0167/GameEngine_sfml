#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"

using namespace sf;
using namespace std;

class Scene // needs to be a singleton and have an observer pattern
{
public:
    Scene(Scene& other) = delete; // Singletons should not be assignable

    void operator=(const Scene&) = delete;

   // RenderTarget* window;
    static Scene* GetInstance();

    vector<GameObject*> m_GameObjects;

    void DebugInfo();
    void AddGameObject(GameObject& drawable);
    void RemoveGameObject(GameObject& _drawable);
    const vector<GameObject*>& GetGameObjects();

protected:
    static Scene* _Scene;

    Scene() {};

};
