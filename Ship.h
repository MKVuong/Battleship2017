/****************************************************************
FILE:		Ship.h
AUTHOR:		Kristein Minh Vuong

PURPOSE:	Ship objects are created to define their name, length,
			and vector of coordinates that they each individually cover.
****************************************************************/
#ifndef SHIP_H
#define SHIP_H

#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include "Point.h"
#include "Constants.h"

using namespace std;

class Ship
{
private:
	int length;	
	string name;

public:
	vector<Point> coords;	//all coordinate locations a ship object occupies

	Ship();
	Ship(int length, string);
	~Ship();

	//Accessor methods
	int getLength();
	string getName();

	//Mutator methods
	void setLength(int);
	void setName(string);
};
#endif