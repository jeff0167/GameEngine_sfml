#include "Rigidbody.h"

using namespace sf;
using namespace std;

void Rigidbody::AddForce(float force) 
{
	
}

double Rigidbody::Magnitude() 
{
	return sqrt(pow(velocity.x, 2) + pow(velocity.y, 2));
}
