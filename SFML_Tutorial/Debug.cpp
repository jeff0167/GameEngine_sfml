#include "Debug.h"

using namespace sf;
using namespace std;

Debug* Debug::_debug = nullptr;

Debug* Debug::GetInstance(const string& value)
{
	if (_debug == nullptr) {
		_debug = new Debug(value); // we just instantiated our one debug object, now start it's thread
	}
	return _debug;
}