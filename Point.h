/****************************************************************
FILE:		Point.h
AUTHOR:		Kristein Minh Vuong

PURPOSE:	Defines x,y coordinates within vectors
****************************************************************/
#ifndef POINT_H
#define POINT_H

struct Point
{
	//x & y coordinates of a point in grid
	int x;
	int y;

	Point(int parmx, int parmy)	//or Point(int parmx, int parmy) : x(parmx), y(parmy) {}
	{
		x = parmx;
		y = parmy;
	}
};
#endif 