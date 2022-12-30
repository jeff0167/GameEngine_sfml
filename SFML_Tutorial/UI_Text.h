#pragma once
#include <SFML/Graphics.hpp>
#include "Component.h"

using namespace sf;
using namespace std;

class UI_Text : public Component, public Text // could you somehow set it to call base in initialization?!?! tohugh it would have two different bases?
{
public:
	UI_Text() {};
	UI_Text(Vector2f pos, const string& _string, unsigned int characterSize = 30)
	{
		setPosition(pos);
		setString(_string);	
		//setFont(font); // can't set it to a default value, gets trouble with there being null pointer and reading violation
		setCharacterSize(characterSize);
	};

};

