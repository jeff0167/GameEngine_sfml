#include "Debug.h"

using namespace sf;
using namespace std;

Debug* Debug::_debug = nullptr;

Debug* Debug::GetInstance()
{
	if (_debug == nullptr) {
		_debug = new Debug(); // we just instantiated our one debug object, now start it's thread
	}
	return _debug;
}