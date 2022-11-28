#include "Rigidbody.h"
#include "Mathf.h"

using namespace sf;
using namespace std;

void Rigidbody::AddForce(float force) 
{
	
}

double Rigidbody::Magnitude() 
{
	return Mathf::Magnitude(velocity);
}
