#pragma once
#include <SFML/Graphics.hpp>
#include "Component.h" // used for particlesystem to inherit component wihtout compile issues

using namespace sf;
using namespace std;
 
class Canvas 
{
public:
    Canvas(Canvas& other) = delete;

    void operator=(const Canvas&) = delete;

    RenderTarget* window;
    static Canvas *GetInstance();

	vector<Drawable*> drawables;

    void DrawCanvas();
	void AddDrawable(Drawable& drawable);
	void RemoveDrawable(Drawable& _drawable);
    const vector<Drawable*>& GetDrawables();
    void AddWindow(RenderTarget& renderTarget);

protected:
	static Canvas* _canvas;

    Canvas() {};

};