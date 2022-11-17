#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Component
{
public:
	int Member;

	Component();
	virtual void AbstractNullFunc() 
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
