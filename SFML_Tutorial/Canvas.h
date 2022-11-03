#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>
#include <iterator>
#include <list>

using namespace sf;
using namespace std;

class Canvas
{
public:
	vector<Drawable*> drawables;

	Canvas();
	void AddDrawable(Drawable &drawable);
	void RemoveDrawable(Drawable &_drawable);
	void DrawCanvas(RenderWindow window);

private:

};
