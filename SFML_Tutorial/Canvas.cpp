#include "Canvas.h"

using namespace sf;
using namespace std;

Canvas* Canvas::_canvas = nullptr; // it just has this as a property!? outside class and function scope?

Canvas *Canvas::GetInstance(const string& value)
{
	if (_canvas == nullptr) {
		_canvas = new Canvas(value);
	}
	return _canvas;
}

void Canvas::AddDrawable(Drawable &drawable) 
{
	drawables.push_back(&drawable);
}

void Canvas::RemoveDrawable(Drawable &_drawable)
{
	for (size_t i = 0; i < drawables.size(); i++)
	{
		if (drawables[i] == &_drawable)
		{
			drawables.erase(next(drawables.begin(), i), next(drawables.begin(), i + 1)); 
		}
	}
}

vector<Drawable*> Canvas::GetDrawables() 
{
	return drawables;
}

void Canvas::DrawCanvas(RenderTarget &renderTarget) 
{
	for (size_t i = 0; i < drawables.size(); i++)
	{
		renderTarget.draw(*drawables[i]);
	}
}