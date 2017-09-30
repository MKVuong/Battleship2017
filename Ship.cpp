#include "stdafx.h"
#include "Ship.h"


Ship::Ship()
{
	hits = 0;
	length = 0;
	name = "namelessShip";
}

Ship::Ship(int newLength, string newName)
{
	hits = 0;
	length = newLength;
	name = newName;
}

Ship::~Ship()
{
}

int Ship::getHits()
{
	return hits;
}

void Ship::setHits(int newHits)
{
	hits = newHits;
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
