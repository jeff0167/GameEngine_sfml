#pragma once
#include <SFML/Graphics.hpp>

#define MousePos MousePoint::GetInstance()->mousePixel

using namespace sf;

class MousePoint
{
public:
	RectangleShape mousePixel = RectangleShape(Vector2f(1, 1));

    MousePoint(MousePoint& other) = delete;

    void operator=(const MousePoint&) = delete;

    static MousePoint* GetInstance();

protected:
    static MousePoint* _MousePoint;
    MousePoint() {};

};