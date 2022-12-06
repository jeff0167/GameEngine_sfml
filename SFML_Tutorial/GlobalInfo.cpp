#include "GlobalInfo.h"

using namespace std;

GlobalInfo* GlobalInfo::_GlobalInfo = nullptr;

GlobalInfo* GlobalInfo::GetInstance()
{
	if (_GlobalInfo == nullptr) {
		_GlobalInfo = new GlobalInfo();
	}
	return _GlobalInfo;
}