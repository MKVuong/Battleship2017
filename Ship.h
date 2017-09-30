#pragma once
#ifndef SHIP_H
#define SHIP_H

#include <string>
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

//Dimensions of grid
static const int rows = 10;
static const int cols = 10;

struct Point
{
	//x & y coordinates of a point in grid
	int x;
	int y;

	//Default Constructor initializes coords to out of bounds
	/*Point()
	{
		x = rows;
		y = cols;
	}*/
	Point(int parmx, int parmy)
	{
		x = parmx;
		y = parmy;
	}
	//or
	//Point(int parmx, int parmy) : x(parmx), y(parmy) {}
};

//Submarine, 
class Ship
{
private:
	int hits;
	int length;	
	string name;

public:
	//Point points[5];	//max possibility for 1 ship
	vector<Point> coords;

	Ship();
	Ship(int length, string);
	~Ship();

	//Accessor methods
	int getHits();
	int getLength();
	string getName();

	//Mutator methods	
	void setHits(int);
	void setLength(int);
	void setName(string);
};

#endif