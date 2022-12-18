#pragma once
#include <SFML/Graphics.hpp>
#include "Component.h"

using namespace sf;
using namespace std;

class UI_Button : public Component
{
public:
	RectangleShape* rect;

	UI_Button() {};

	UI_Button(RectangleShape& _rect) :
		rect(&_rect)
	{

	};

	// hmm needs to be clickable
	// it needs an area and when you click with mouse you check if the mouse is within that area
	// say you click the mouse button, then you need to check all clickable objects for their area


private:

};