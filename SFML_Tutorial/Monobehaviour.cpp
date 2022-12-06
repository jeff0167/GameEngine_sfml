#include "Monobehaviour.h"
#include "GameObject.h"
#include "Debug.h"

using namespace sf;

Monobehaviour* Monobehaviour::_monobehaviour = nullptr;

Monobehaviour* Monobehaviour::GetInstance()
{
	if (_monobehaviour == nullptr) {
		_monobehaviour = new Monobehaviour();
	}
	return _monobehaviour;
}

GameObject Monobehaviour::Instantiate(GameObject gameObject)
{
	return gameObject;
}

GameObject Monobehaviour::Instantiate(GameObject gameObject, Transformable transform)
{
	return gameObject;
}

void Monobehaviour::Destroy(GameObject* gameObject)
{
	delete &gameObject; // no idea how deleting is supposed to be done, though should be possible
}

void Monobehaviour::Invoke(function<void()> function, float callDelay)
{
	thread t(DelayedCall, function, callDelay);
	t.detach();
}

void Monobehaviour::DelayedCall(function<void()> function, float callDellay)
{
	this_thread::sleep_for(chrono::milliseconds((long)callDellay));
	function();
}