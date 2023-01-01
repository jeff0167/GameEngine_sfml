#pragma once
#include <SFML/Graphics.hpp>
#include "Component.h" // used for particlesystem to inherit component wihtout compile issues

#define Renderer Canvas::GetInstance()

using namespace sf;
using namespace std;
 
class Canvas
{
public:
    Canvas(Canvas& other) = delete;

    void operator=(const Canvas&) = delete;

    RenderTarget* window;
    static Canvas* GetInstance();

    vector<Drawable*> drawables;

    void DrawCanvas();
    void AddDrawable(Drawable& drawable, int layerNr = 5);
    void RemoveDrawable(Drawable& _drawable);
    const vector<Drawable*>& GetDrawables();
    void AddWindow(RenderTarget& renderTarget);

    // size of 10, how would I do this dynamicly?!? or say you would add another layer but the user would never have to do it manualy?
    vector<vector<Drawable*>> drawablesLayers = { {},{},{},{},{},{},{},{},{},{} }; 
     
    void ChangeDrawableLayer(Drawable& _drawable, int layerNr) // maybe return bool depending on if it was succesfull?
    {
        for (auto& layer : drawablesLayers)
        {
            for (size_t i = 0; i < layer.size(); i++)
            {
                if (layer[i] == &_drawable) layer.erase(next(layer.begin(), i), next(layer.begin(), i + 1));
            }
        }

        drawablesLayers[layerNr].push_back(&_drawable); // suppose that does it, yes, yes it does
    }

protected:
	static Canvas* _canvas;

    Canvas() {};

};