#pragma once
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
	vector<Point> coords;

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