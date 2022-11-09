#include "Rigidbody.h"

using namespace sf;
using namespace std;

void Rigidbody::AddForce() 
{
	
}

Rigidbody::Rigidbody(Transformable& _transform)
{
	transform = &_transform;
}
