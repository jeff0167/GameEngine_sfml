#include "Canvas.h"

using namespace sf;
using namespace std;

Canvas* Canvas::_canvas = nullptr;

Canvas *Canvas::GetInstance()
{
	if (_canvas == nullptr) {
		_canvas = new Canvas();
	}
	return _canvas;
}

void Canvas::AddWindow(RenderTarget& renderTarget)
{
	window = &renderTarget;
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

const vector<Drawable*>& Canvas::GetDrawables() 
{
	return drawables;
}

void Canvas::DrawCanvas() 
{
	for (size_t i = 0; i < drawables.size(); i++)
	{
		window->draw(*drawables[i]);
	}
}