#include "Canvas.h"

using namespace sf;

Canvas::Canvas()
{
}

void Canvas::AddDrawable(Drawable &drawable) // I need a monobehaviour, dude it makes sense, dont judge the name, to instantiate gameobjects with and other things
{
	drawables.push_back(&drawable);
}

void Canvas::RemoveDrawable(Drawable &_drawable)
{
	for (size_t i = 0; i < drawables.size(); i++)
	{
		/*if (texture == _texture) // should we make our own texture class so we can check the class type ref, could we check another way?
		{
			textures.remove(texture);
		}*/
	}
}

void Canvas::DrawCanvas(RenderWindow window)
{
	for (size_t i = 0; i < drawables.size(); i++)
	{
		window.draw(*drawables[i]);
	}
}