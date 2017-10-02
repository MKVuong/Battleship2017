#pragma once
#ifndef POINT_H
#define POINT_H

//Dimensions of grid
static const int rows = 10;
static const int cols = 10;

struct Point
{
	//x & y coordinates of a point in grid
	int x;
	int y;

	Point(int parmx, int parmy)
	{
		x = parmx;
		y = parmy;
	}
	//or
	//Point(int parmx, int parmy) : x(parmx), y(parmy) {}
};

#endif POINT_H