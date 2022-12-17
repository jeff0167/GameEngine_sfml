#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"

#define Scene SceneWindow::GetInstance()

using namespace sf;
using namespace std;

class SceneWindow 
{
public:
    SceneWindow(SceneWindow& other) = delete;

    void operator=(const SceneWindow&) = delete;

    static SceneWindow* GetInstance();

    vector<GameObject*> m_GameObjects;

    void DebugInfo();
    void AddGameObject(GameObject& drawable);
    void RemoveGameObject(GameObject& _drawable);
    const vector<GameObject*>& GetGameObjects();

    void DisplaySceneWindow(RenderWindow& window);

protected:
    static SceneWindow* _SceneWindow;

    SceneWindow() {};

};
