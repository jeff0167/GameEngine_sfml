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

void Canvas::ChangeDrawableLayer(Drawable& _drawable, int layerNr)
{
	RemoveDrawable(_drawable);

	drawablesLayers[layerNr].push_back(&_drawable);
}

void Canvas::AddDrawable(Drawable& drawable, int layerNr) 
{
	drawablesLayers[layerNr].push_back(&drawable);
}

void Canvas::RemoveDrawable(Drawable &_drawable)
{
	for (auto& layer : drawablesLayers)
	{
		for (size_t i = 0; i < layer.size(); i++)
		{
			if (layer[i] == &_drawable)
			{
				layer.erase(next(layer.begin(), i), next(layer.begin(), i + 1));
				return;
			}
		}
	}
}

const vector<vector<Drawable*>>& Canvas::GetDrawables() 
{
	return drawablesLayers;
}

void Canvas::DrawCanvas() 
{
	for (auto& layer : drawablesLayers) 
	{
		for (auto& drawable : layer)
		{
			if (drawable == nullptr) continue;
			window->draw(*drawable);
		}
	}
}