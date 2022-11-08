#include "Input.h"

using namespace sf;
using namespace std;

static bool notPressedKeys[100] = { false };
static bool pressedNotReleased[100] = { false }; // suppose you could do the same with mouse input?

static bool keyEvent[3] = {false};

bool Input::GetKey(Keyboard::Key key, Keycode keycode) 
{
	keyEvent[0] = GetKeyDown(key);
	keyEvent[1] = GetKeyHeld(key);
	keyEvent[2] = GetKeyUp(key);

	return keyEvent[keycode];
}

bool Input::GetKeyDown(Keyboard::Key key)
{
	if (Keyboard::isKeyPressed(key))
	{
		pressedNotReleased[key] = true;
		if (!notPressedKeys[key])
		{
			return notPressedKeys[key] = true;
		}		
	}
	return false;
}

bool Input::GetKeyHeld(Keyboard::Key key) 
{
	return Keyboard::isKeyPressed(key);
}

bool Input::GetKeyUp(Keyboard::Key key)
{
	if (!Keyboard::isKeyPressed(key) && pressedNotReleased[key]) // is key being held down, no, but it's held boolean is still true
	{
		pressedNotReleased[key] = false;
		notPressedKeys[key] = false;
		return true;
	}
	return false;
}