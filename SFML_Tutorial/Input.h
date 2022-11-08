#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>
#include <iterator>
#include <list>

using namespace sf;
using namespace std;

class Input // needs to be a singleton and have an observer pattern
{
public:
    enum Keycode
    {
        KeyDown,
        KeyHeld,
        KeyUp
    };
    static bool GetKey(Keyboard::Key key, Keycode keykode);

private:
    static bool GetKeyDown(Keyboard::Key key);
    static bool GetKeyHeld(Keyboard::Key key);
    static bool GetKeyUp(Keyboard::Key key);
};
