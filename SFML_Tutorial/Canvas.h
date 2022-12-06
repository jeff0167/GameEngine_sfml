#pragma once
#include <SFML/Graphics.hpp>
#include "Component.h" // used for particlesystem to inherit component wihtout compile issues

using namespace sf;
using namespace std;
 
class Canvas // needs to be a singleton and have an observer pattern
{
public:
    Canvas(Canvas& other) = delete; // Singletons should not be assignable

    void operator=(const Canvas&) = delete;
    
     // This is the static method that controls the access to the singleton
     // instance. On the first run, it creates a singleton object and places it
     // into the static field. On subsequent runs, it returns the client existing
     // object stored in the static field

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