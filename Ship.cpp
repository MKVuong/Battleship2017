#include "stdafx.h"
#include "Ship.h"


Ship::Ship()
{
	length = 0;
	name = "namelessShip";
}

Ship::Ship(int newLength, string newName)
{
	length = newLength;
	name = newName;
}

Ship::~Ship()
{
}


int Ship::getLength()
{
	return length;
}

void Ship::setLength(int newLength)
{
	length = newLength;
}

string Ship::getName()
{
	return name;
}

void Ship::setName(string newName)
{
	name = newName;
}
