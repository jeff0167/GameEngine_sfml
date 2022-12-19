#include "MousePoint.h"

MousePoint* MousePoint::_MousePoint = nullptr;

MousePoint* MousePoint::GetInstance()
{
	if (_MousePoint == nullptr)
	{
		_MousePoint = new MousePoint();
	}
	return _MousePoint;
}
