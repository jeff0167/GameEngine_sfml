#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>
#include <iterator>
#include <list>

using namespace sf;
using namespace std;
 
class Canvas // needs to be a singleton and have an observer pattern
{
public:
    Canvas(Canvas& other) = delete;
    /**
     * Singletons should not be assignable.
     */
    void operator=(const Canvas&) = delete;
    /**
     * This is the static method that controls the access to the singleton
     * instance. On the first run, it creates a singleton object and places it
     * into the static field. On subsequent runs, it returns the client existing
     * object stored in the static field.
     */

    static Canvas *GetInstance(const string& value);

    string value() const {
        return value_;
    }

	vector<Drawable*> drawables;

	void AddDrawable(Drawable &drawable);
	void RemoveDrawable(Drawable &_drawable);
    vector<Drawable*> GetDrawables();
    void DrawCanvas(RenderTarget &renderTarget);

protected:
    string value_;
	static Canvas* _canvas;

    Canvas(const string& value) : value_(value) {}; // constructor

};
