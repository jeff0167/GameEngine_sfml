#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>
#include <iterator>
#include <list>

using namespace sf;
using namespace std;

class Component
{
public:
	int Member;

	Component();
	virtual void Yo() 
	{

	}

	bool operator == (const Component& Ref) const
	{
		return(this->Member == Ref.GetMember());
	}

	const int GetMember() const
	{
		return(this->Member);
	}

private:

};
